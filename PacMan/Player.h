#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GlobalData.h>
#include <Utils.h>
#include <LTexture.h>


class Player
{
    //This Player class has rect collision detector
public:
    //Initializes the variables
    Player();

    //Init the Player
    bool init(SDL_Renderer* renderer, SDL_Window* window);

    //Takes key presses and adjusts the Player's velocity
    void handleEvent(SDL_Event& e);

    //Moves the Player
    void update();


    //Shows the Player on the screen
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
    //Moves the collision circle relative to the Player's offset
    void shiftColliders();
private:

    //consts
    const float TEXTURE_SCALE = 2.0f;
    const float PLAYER_VEL = 1.0f;
    const float PLAYER_WIDTH = 16 * TEXTURE_SCALE;
    const float PLAYER_HEIGHT = 16 * TEXTURE_SCALE;

    //The X and Y offsets of the Player
    float _xPos = 10;
    float _yPos = 10;

    //The velocity of the Player
    float _xVel = 0;
    float _yVel = 0;

    //Players texture
    LTexture _PlayerTexture;

    
    //Players collision circle
    SDL_FRect _collisionRect = { 0,0, PLAYER_WIDTH, PLAYER_HEIGHT };

};

