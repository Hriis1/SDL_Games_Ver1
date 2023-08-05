#include "Player.h"

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
        //case SDLK_UP: _yVel -= PLAYER_VEL; break;
        //case SDLK_DOWN: _yVel += PLAYER_VEL; break;
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
        //case SDLK_UP: _yVel += PLAYER_VEL; break;
        //case SDLK_DOWN: _yVel -= PLAYER_VEL; break;
        case SDLK_LEFT: _xVel += PLAYER_VEL; break;
        case SDLK_RIGHT: _xVel -= PLAYER_VEL; break;
        }
    }
}

void Player::update(std::vector<Tile*>& tiles, float gravity)
{
    //Move the dot left or right
    _xPos += _xVel;
    shiftColliders();

    //If the dot collided or went too far to the left or right
    if ((_xPos < 0) || (_xPos + _collisionRect.w > LEVEL_WIDTH))
    {
        //Move back
        _xPos -= _xVel;
        shiftColliders();
    }

    //Handle X collision with tiles
    for (size_t i = 0; i < tiles.size(); i++)
    {
        // if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
        {
            if (checkCollision(_collisionRect, tiles[i]->getBox()))
            {
                //Move back
                _xPos -= _xVel;
                shiftColliders();
            }
        }
    }

    //Move the dot up or down
    _yPos += _yVel;
    shiftColliders();

    //If the dot collided or went too far up or down
    if ((_yPos < 0) || (_yPos + _collisionRect.h > LEVEL_HEIGHT))
    {
        //Move back
        _yPos -= _yVel;
        shiftColliders();
    }

    //Handle Y collision with tiles
    bool onground = false;
    for (size_t i = 0; i < tiles.size(); i++)
    {
        // if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
        {
            if (checkCollision(_collisionRect, tiles[i]->getBox()))
            {
                //Move back
                _yPos -= _yVel;
                shiftColliders();

                //set flag and stop yvel
                onground = true;
                _yVel = 0.0f;
            }
        }
    }

    _grounded = onground;
    if (!_grounded)
    {
        _yVel += gravity;
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
