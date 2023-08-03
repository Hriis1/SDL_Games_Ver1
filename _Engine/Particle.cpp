#include "Particle.h"

bool Particle::_inited = false;
LTexture Particle::_shimmerTexture;
LTexture Particle::_redTexture;
LTexture Particle::_greenTexture;
LTexture Particle::_blueTexture;

Particle::Particle(int x, int y)
{
    //Set offsets
    _xPos = x - 5 + (rand() % 25);
    _yPos = y - 5 + (rand() % 25);

    //Initialize animation
    _frame = rand() % 5;

    //Set type
    switch (rand() % 3)
    {
    case 0: _texture = &_redTexture; break;
    case 1: _texture = &_greenTexture; break;
    case 2: _texture = &_blueTexture; break;
    }
}

void Particle::render()
{
    //Show image
    _texture->render(_xPos, _yPos);

    //Show shimmer
    if (_frame % 2 == 0)
    {
        _shimmerTexture.render(_xPos, _yPos);
    }

    //Animate
    _frame++;
}

bool Particle::isDead()
{
    return _frame > _MAX_PARTICLE_LIFETIME;
}
