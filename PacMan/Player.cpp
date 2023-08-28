#include "Player.h"

Player::Player()
{
    //Set collision circle size
    _collisionCircle.r = Player_WIDTH / 2;

    //Move collider relative to the circle
    shiftColliders();
}

bool Player::init(SDL_Renderer* renderer, SDL_Window* window)
{
    _PlayerTexture.initRenderer(renderer);

    if(!_PlayerTexture.loadFromFile("textures/Player.bmp", window))
    {
        printf("Could not load Player texture\n");
        return false;
    }

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
        case SDLK_UP: _yVel -= Player_VEL; break;
        case SDLK_DOWN: _yVel += Player_VEL; break;
        case SDLK_LEFT: _xVel -= Player_VEL; break;
        case SDLK_RIGHT: _xVel += Player_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: _yVel += Player_VEL; break;
        case SDLK_DOWN: _yVel -= Player_VEL; break;
        case SDLK_LEFT: _xVel += Player_VEL; break;
        case SDLK_RIGHT: _xVel -= Player_VEL; break;
        }
    }
}

void Player::update()
{
    //Move the Player left or right
    _xPos += _xVel;
    shiftColliders();


    //Move the Player up or down
    _yPos += _yVel;
    shiftColliders();

    
}

void Player::render(int camX, int camY)
{
    _PlayerTexture.render(_xPos - _collisionCircle.r - camX, _yPos - _collisionCircle.r - camY);
}

void Player::shiftColliders()
{
    _collisionCircle.x = _xPos;
    _collisionCircle.y = _yPos;
}
