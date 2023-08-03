#pragma once
#include <SDL/SDL.h>
#include "LTexture.h"

class Particle
{
public:
    //Initialize position and animation
    Particle(int x, int y);

    //Shows the particle
    void render();

    //Checks if particle is dead
    bool isDead();

    //static
    static bool initParticles(SDL_Renderer* renderer, SDL_Window* window)
    {
        if (!_inited)
        {
            //Init textures renderer
            _shimmerTexture.initRenderer(renderer);
            _redTexture.initRenderer(renderer);
            _greenTexture.initRenderer(renderer);
            _blueTexture.initRenderer(renderer);

            //Load shimmer
            if (!_shimmerTexture.loadFromFile("../_Engine/shimmer.bmp", window))
            {
                return false;
            }

            //Load red
            if (!_redTexture.loadFromFile("../_Engine/red.bmp", window))
            {
                return false;
            }

            //Load green
            if (!_greenTexture.loadFromFile("../_Engine/green.bmp", window))
            {
                return false;
            }

            //Load blue
            if (!_blueTexture.loadFromFile("../_Engine/blue.bmp", window))
            {
                return false;
            }

            //Set texturs alpha
            _shimmerTexture.setAlpha(192);
            _redTexture.setAlpha(192);
            _greenTexture.setAlpha(192);
            _blueTexture.setAlpha(192);
            


            _inited = true;
        }

        return true;
    }

private:
    //particle lifetime
    const int _MAX_PARTICLE_LIFETIME = 50;

    //Offsets
    int _xPos = 0;
    int _yPos = 0;

    //Current frame of animation
    int _frame = 0;

    //Type of particle
    LTexture* _texture = NULL;

    //static
    static bool _inited;

    static LTexture _shimmerTexture;
    static LTexture _redTexture;
    static LTexture _greenTexture;
    static LTexture _blueTexture;
};

