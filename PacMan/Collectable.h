#pragma once
#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <Utils.h>
#include <LTexture.h>

#include "PacManData.h"

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
    Collectable(CollectableType type ,float xPos, float yPos);

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
    const float TEXTURE_SCALE = 2.0f;
    const float Collectable_WIDTH = 16 * TEXTURE_SCALE;
    const float Collectable_HEIGHT = 16 * TEXTURE_SCALE;

    //The type of the collectable
    CollectableType _type;

    //The X and Y offsets of the Collectable
    float _xPos = 0;
    float _yPos = 0;

    //Collectables collision circle
    SDL_FRect _collisionRect = { 0,0, Collectable_WIDTH, Collectable_HEIGHT };

    //Collectables texture
    LTexture _collectableSprite;


};



