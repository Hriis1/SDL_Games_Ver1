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

    //Init the Collectable textures
    bool init(SDL_Renderer* renderer, SDL_Window* window);


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
private:

    //consts
    const float TEXTURE_SCALE = 1.0f;
    const float COLLECTABLE_WIDTH = 16 * TEXTURE_SCALE;
    const float COLLECTABLE_HEIGHT = 16 * TEXTURE_SCALE;

    //The X and Y offsets of the Collectable
    float _xPos = 0;
    float _yPos = 0;

    //The type of the collectable
    CollectableType _type;

    //Collectables collision circle
    SDL_FRect _collisionRect = { 0,0, COLLECTABLE_WIDTH, COLLECTABLE_HEIGHT };

    //Collectables texture
    static LTexture _smallCollectableSprite;
    static LTexture _bigCollectableSprite;

    //Spriteclips
    int _animationFrame = 0;
    SDL_Rect _spriteClips[COLLECTABLE_ANIMATION_FRAMES];

};



