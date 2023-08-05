#include "Player.h"
#include <iostream>

Player::Player()
{
	//Move collider relative to the circle
	shiftColliders();
}

bool Player::init(SDL_Renderer* renderer, SDL_Window* window)
{
    _texture.initRenderer(renderer);

    if (!_texture.loadFromFile("textures/player/poki.png", window))
    {
        printf("Could not load PLAYER texture\n");
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

void Player::update(std::vector<Tile*>& tiles, float gravity)
{
    /*if (!_grounded)
    {
        _yVel += gravity;
    }*/
    //Move the dot x or right
    _xPos += _xVel;
    _yPos += _yVel;
    shiftColliders();

    std::cout << "x= " << _xPos << " y= " << _yPos << std::endl;

    //If the dot collided or went too far to the x or right
    if ((_xPos < 0) || (_xPos + _collisionRect.w > LEVEL_WIDTH))
    {
        //Move back
        _xPos -= _xVel;
        shiftColliders();
    }

    //If the dot collided or went too far up or down
    if ((_yPos < 0) || (_yPos + _collisionRect.h > LEVEL_HEIGHT))
    {
        //Move back
        _yPos -= _yVel;
        shiftColliders();
    }

    //Handle collision with tiles
    for (size_t i = 0; i < tiles.size(); i++)
    {
        if (checkCollision(_collisionRect, tiles[i]->getBox())) 
        {
            //Player box values
            float playerleft = _collisionRect.x;
            float playerRight = _collisionRect.x + _collisionRect.w;
            float playerTop = _collisionRect.y;
            float playerBot = _collisionRect.y + _collisionRect.h;

            //Tile box values
            float tileleft = tiles[i]->getBox().x;
            float tileRight = tiles[i]->getBox().x + tiles[i]->getBox().w;
            float tileTop = tiles[i]->getBox().y;
            float tileBot = tiles[i]->getBox().y + tiles[i]->getBox().h;

            int horizontalDistance = std::min(std::abs(playerRight - tileleft),
                std::abs(playerleft - tileRight));

            int verticalDistance = std::min(std::abs(playerBot - tileTop),
                std::abs(playerTop - tileBot));

            if (horizontalDistance < verticalDistance) {
                // Resolve horizontal collision
                float box1CenterX = playerleft + (_collisionRect.w / 2.0f);
                float box2CenterX = tileleft + (tiles[i]->getBox().w / 2.0f);
                if (box1CenterX < box2CenterX) //coming from the left
                {
                    _xPos += tileleft - playerRight;
                }
                else if (box1CenterX > box2CenterX) {
                    _xPos += tileRight - playerleft;
                }
            }
            else if(horizontalDistance > verticalDistance) {
                float box1CenterY = playerTop + (_collisionRect.h / 2.0f);
                float box2CenterY = tileTop + (tiles[i]->getBox().h / 2.0f);
                // Resolve vertical collision
                if (box1CenterY < box2CenterY) //coming from the top
                {
                    _yPos = tileTop - _collisionRect.h;

                    //Only set grounded to true if the box is colliding with a box below it
                    _grounded = true;
                }
                else {
                    _yPos = tileBot;
                }
            }
            shiftColliders();
        }
    }

    
}

void Player::render(int camX, int camY)
{
    _texture.render(_xPos - camX, _yPos - camY);
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}
