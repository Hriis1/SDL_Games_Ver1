#include "Player.h"
#include <iostream>

Player::Player()
{
	//Move collider relative to the circle
	shiftColliders();
}

bool Player::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //Init textures
    _textureStandingStill.initRenderer(renderer);
    if (!_textureStandingStill.loadFromFile("textures/player/poki.png", window))
    {
        printf("Could not load PLAYER texture\n");
        return false;
    }

    //Set the current texture ptr
    _currentTexture = &_textureStandingStill;

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
        case SDLK_UP: jump(PLAYER_JUMP_AMOUNT); break;
        case SDLK_LEFT: 
            _xVel -= PLAYER_VEL;
            //Change the textures direction
            _facingRight = false;
            break;
        case SDLK_RIGHT: 
            _xVel += PLAYER_VEL;
            //Change the textures direction
            _facingRight = true;
            break;
         
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: _xVel += PLAYER_VEL; break;
        case SDLK_RIGHT: _xVel -= PLAYER_VEL; break;
        }
    }
}

void Player::update(std::vector<Tile*>& tiles, float gravity, float deltaTime)
{
    //Handle gravity
    if (!_grounded)
    {
        _yVel += gravity;
    }
    else
    {
        if (_yVel > 0)
            _yVel = 0;
    }

    //Move the dot x or right
    _xPos += _xVel * deltaTime;
    _yPos += _yVel * deltaTime;
    shiftColliders();

    //std::cout << "x= " << _xPos << " y= " << _yPos << std::endl;

    //If the dot collided or went too far to the x or right
    if ((_xPos < 0) || (_xPos + _collisionRect.w > LEVEL_WIDTH))
    {
        //Move back
        _xPos -= _xVel * deltaTime;
        shiftColliders();
    }

    //If the dot collided or went too far up or down
    if ((_yPos < 0) || (_yPos + _collisionRect.h > LEVEL_HEIGHT))
    {
        //Move back
        _yPos -= _yVel * deltaTime;
        shiftColliders();
    }

    //Handle collision with tiles
    bool onGround = false;
    for (size_t i = 0; i < tiles.size(); i++)
    {
        if (checkCollision(_collisionRect, tiles[i]->getBox())) 
        {
            //Player box values
            int playerleft = _collisionRect.x;
            int playerRight = _collisionRect.x + _collisionRect.w;
            int playerTop = _collisionRect.y;
            int playerBot = _collisionRect.y + _collisionRect.h;

            //Tile box values
            int tileleft = tiles[i]->getBox().x;
            int tileRight = tiles[i]->getBox().x + tiles[i]->getBox().w;
            int tileTop = tiles[i]->getBox().y;
            int tileBot = tiles[i]->getBox().y + tiles[i]->getBox().h;

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

                    //Only set onGround to true if the box is colliding with a box below it
                    onGround = true;
                }
                else {
                    _yPos = tileBot;
                }
            }
            shiftColliders();
        }
    }

    //Set the actual variable to what we got from the loop
    _grounded = onGround;
       

    
}

void Player::render(int camX, int camY)
{
    if(_facingRight)
        _currentTexture->render(_xPos - camX, _yPos - camY, NULL, 1.0f, 0.0, NULL, SDL_FLIP_NONE);
    else
        _currentTexture->render(_xPos - camX, _yPos - camY, NULL, 1.0f, 0.0, NULL, SDL_FLIP_HORIZONTAL);
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}

void Player::jump(float jump_amount)
{
    if (_grounded)
    {
        _yVel -= jump_amount;
    }
}
