#pragma once
#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <Utils.h>
#include <LTexture.h>

#include "PacManData.h"

const int COLLECTABLE_ANIMATION_FRAMES = 8;

enum class CollectableType
{
    SMALL = 0, BIG
};

class Collectable
{
    //This Collectable class has rect collision detector
public:
    
public:
    //Initializes the variables
    Collectable(float xPos, float yPos, CollectableType type = CollectableType::SMALL);

    //Shows the Collectable on the screen
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

    CollectableType getType() const
    {
        return _type;
    }

    //static
    //Init the Collectable textures
    static bool init(SDL_Renderer* renderer, SDL_Window* window);

private:
    //Moves the collision rect relative to the Player's offset
    void shiftColliders();

private:

    

    //The X and Y offsets of the Collectable
    float _xPos = 0;
    float _yPos = 0;

    //The type of the collectable
    CollectableType _type;

    //Collectables collision circle
    SDL_FRect _collisionRect = { 0,0, COLLECTABLE_WIDTH, COLLECTABLE_HEIGHT };

    //animation frames
    int _animationFrame = 0;

    //static
    //consts
    static const float TEXTURE_SCALE;
    static const float COLLECTABLE_WIDTH;
    static const float COLLECTABLE_HEIGHT;

    //sprite clips
    static SDL_Rect _spriteClips[COLLECTABLE_ANIMATION_FRAMES];

    //Collectables texture
    static LTexture _smallCollectableSpriteSheet;
    static LTexture _bigCollectableSpriteSheet;

};



