#include "Dot.h"

Dot::Dot(SDL_Renderer* renderer, SDL_Window* window)
{
    _dotTexture.initRenderer(renderer);
    _dotTexture.loadFromFile("dot.bmp", window);

    //Set collision circle size
    _collisionCircle.r = DOT_WIDTH / 2;

    //Move collider relative to the circle
    shiftColliders();
}

void Dot::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: _yVel -= DOT_VEL; break;
        case SDLK_DOWN: _yVel += DOT_VEL; break;
        case SDLK_LEFT: _xVel -= DOT_VEL; break;
        case SDLK_RIGHT: _xVel += DOT_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: _yVel += DOT_VEL; break;
        case SDLK_DOWN: _yVel -= DOT_VEL; break;
        case SDLK_LEFT: _xVel += DOT_VEL; break;
        case SDLK_RIGHT: _xVel -= DOT_VEL; break;
        }
    }
}

void Dot::update(std::vector<SDL_FRect>& boxes, std::vector<Circle<float>> circles, Tile* tiles[])
{
    //Move the dot left or right
    _xPos += _xVel;
    shiftColliders();

    //If the dot collided or went too far to the left or right
    if ((_xPos - _collisionCircle.r< 0) || (_xPos + _collisionCircle.r > LEVEL_WIDTH))
    {
        //Move back
        _xPos -= _xVel;
        shiftColliders();
    }

    //Handle X collision with boxes
    for (size_t i = 0; i < boxes.size(); i++)
    {
        if (checkCollision(_collisionCircle, boxes[i]))
        {
            //Move back
            _xPos -= _xVel;
            shiftColliders();
        }
    }

    //Handle X collision with circles
    for (size_t i = 0; i < circles.size(); i++)
    {
        if (checkCollision(_collisionCircle, circles[i]))
        {
            //Move back
            _xPos -= _xVel;
            shiftColliders();
        }
    }

    //Handle X collision with tiles
    for (size_t i = 0; i < TOTAL_TILES; i++)
    {
        if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
        {
            if (checkCollision(_collisionCircle, tiles[i]->getBox()))
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
    if ((_yPos - _collisionCircle.r < 0) || (_yPos + _collisionCircle.r > LEVEL_HEIGHT))
    {
        //Move back
        _yPos -= _yVel;
        shiftColliders();
    }

    //Handle Y collision with boxes
    for (size_t i = 0; i < boxes.size(); i++)
    {
        if (checkCollision(_collisionCircle, boxes[i]))
        {
            //Move back
            _yPos -= _yVel;
            shiftColliders();
        }
    }

    //Handle Y collision with circles
    for (size_t i = 0; i < circles.size(); i++)
    {
        if (checkCollision(_collisionCircle, circles[i]))
        {
            //Move back
            _yPos -= _yVel;
            shiftColliders();
        }
    }

    //Handle Y collision with tiles
    for (size_t i = 0; i < TOTAL_TILES; i++)
    {
        if ((tiles[i]->getType() >= TILE_CENTER) && (tiles[i]->getType() <= TILE_TOPLEFT))
        {
            if (checkCollision(_collisionCircle, tiles[i]->getBox()))
            {
                //Move back
                _yPos -= _yVel;
                shiftColliders();
            }
        }
    }

    
}

void Dot::render(int camX, int camY)
{
    _dotTexture.render(_xPos - _collisionCircle.r - camX, _yPos - _collisionCircle.r - camY);
}

void Dot::shiftColliders()
{
    _collisionCircle.x = _xPos;
    _collisionCircle.y = _yPos;
}
