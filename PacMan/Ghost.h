#pragma once
#pragma once
#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <Utils.h>
#include <LTexture.h>

#include "PacManData.h"

const int Ghost_ANIMATION_FRAMES = 8;

enum class GhostType
{
    SMALL = 0, BIG
};

class Ghost
{
    //This Ghost class has rect collision detector
public:

public:
    //Initializes the variables
    Ghost(float xPos, float yPos, GhostType type = GhostType::SMALL);

    //Shows the Ghost on the screen
    void render(int camX, int camY);

    //getters
    const SDL_FRect& getCollider() const
    {
        return _collisionRect;
    }

    float getXPos() const
    {
        return _xPos;
    }

    float getYPos() const
    {
        return _yPos;
    }

    GhostType getType() const
    {
        return _type;
    }

    //static
    //Init the Ghost textures
    static bool init(SDL_Renderer* renderer, SDL_Window* window);

private:
    //Moves the collision rect relative to the Player's offset
    void shiftColliders();

private:



    //The X and Y offsets of the Ghost
    float _xPos = 0;
    float _yPos = 0;

    //The type of the Ghost
    GhostType _type;

    //Ghosts collision circle
    SDL_FRect _collisionRect = { 0,0, Ghost_WIDTH, Ghost_HEIGHT };

    //animation frames
    int _animationFrame = 0;

    //static
    //consts
    static const float TEXTURE_SCALE;
    static const float Ghost_WIDTH;
    static const float Ghost_HEIGHT;

    //sprite clips
    static SDL_Rect _spriteClips[Ghost_ANIMATION_FRAMES];

    //Ghosts texture
    static LTexture _smallGhostSpriteSheet;
    static LTexture _bigGhostSpriteSheet;

};





