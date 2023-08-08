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
        printf("Could not load _textureStandingStill\n");
        return false;
    }

    _textureMoving.initRenderer(renderer);
    if (!_textureMoving.loadFromFile("textures/player/pokiMoving.png", window))
    {
        printf("Could not load _textureMoving\n");
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
        //case SDLK_UP: jump(PLAYER_JUMP_AMOUNT); break;
        case SDLK_LEFT: 
            _walkingVel -= PLAYER_VEL;
            //Change the textures direction
            _facingRight = false;
            break;
        case SDLK_RIGHT: 
            _walkingVel += PLAYER_VEL;
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
        case SDLK_LEFT: _walkingVel += PLAYER_VEL; break;
        case SDLK_RIGHT: _walkingVel -= PLAYER_VEL; break;
        case SDLK_SPACE: jump(); break;
        }
    }
}

void Player::update(std::vector<Tile*>& tiles, float gravity, float deltaTime)
{
    //updates the texture to be rendered
    updateTexture();

    //charges the jump
    chargeJump(deltaTime);

    //Handle gravity
    if (!_grounded)
    {
        _yVel += gravity;
    }
    else //player is on the ground
    {
        if (_yVel > 0)
            _yVel = 0;
    }

    //Move the player
    _xVel = _grounded ? _jumpingVel + _walkingVel : (_walkingVel/3.0f) + _jumpingVel;  //calculate x velocity
    if (!_chargingJump) //only update the playrs xpos if jump is not being charged
    {
        _xPos += _xVel * deltaTime;
    }
    _yPos += _yVel * deltaTime;
    shiftColliders();

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
    bool bounced = false;
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

                if (!_grounded && !bounced)
                {
                    //bounce the player of the wall if the player is not on the ground
                    _jumpingVel = -(_jumpingVel * 2)/3;
                    bounced = true;
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

    if (_grounded)
    {
        //take away the xvelocity excluding the walking velocity
        _jumpingVel = 0.0f;
   }
       

    
}

void Player::render(int camX, int camY)
{
    if(_facingRight)
        _currentTexture->render(_xPos - camX, _yPos - camY, NULL, 1.0f, 0.0, NULL, SDL_FLIP_NONE);
    else
        _currentTexture->render(_xPos - camX, _yPos - camY, NULL, 1.0f, 0.0, NULL, SDL_FLIP_HORIZONTAL);
}

void Player::updateTexture()
{
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_RIGHT]) //if left arrow or right arrow is being pressed
    {
        _currentTexture = &_textureMoving;
    }
    else if (!_grounded) //also if the player is in the air
    {
        _currentTexture = &_textureMoving;
    }
    else //if the player is on the ground and is not moving
    {
        _currentTexture = &_textureStandingStill;
    }
}

void Player::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}

void Player::chargeJump(float deltaTime)
{
    if (_grounded) //only charge jump if grounded
    {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_SPACE]) //if space is being pressed
        {
            if(_jumpVelocity < MAX_JUMP_VELICTY) //only add to the jump velicty if its less than the max
                _jumpVelocity += JUMP_CHARGE * deltaTime;

            //set the chargingjump flag to true
            _chargingJump = true;
        }
        else //if space is not being pressed
        {
            _chargingJump = false;
        }
    }
    else //player is not on the ground
    {
        //Player cannot be charging jump if not on the ground
        _chargingJump = false;
    }

    
}

void Player::jump()
{
    if (_grounded) //only jump if grounded
    {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_RIGHT]) //right arrow is being pressed when jumping
        {
            _jumpingVel += JUMP_X_VEL;
        }
        else if(state[SDL_SCANCODE_LEFT]) //left arrow is being pressed when jumping
        {
            _jumpingVel -= JUMP_X_VEL;
        }


        _yVel -= _jumpVelocity;
    }

    //Reset the jump velocity
    _jumpVelocity = 0.0f;
}


