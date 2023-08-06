#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GlobalData.h>
#include <Utils.h>
#include <LTexture.h>
#include <Tile.h>



class Player
{
public:
    //Initializes the variables
    Player();

    //Init the dot
    bool init(SDL_Renderer* renderer, SDL_Window* window);

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    //Moves the dot
    //If the function takes a deltaTime the player will be updated independant of framerate 
    void update(std::vector<Tile*>& tiles, float gravity = 0.0f, float deltaTime = 1.0f);


    //Shows the dot on the screen
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
    //Moves the collision rect relative to the offset
    void shiftColliders();
    
    void jump(float jump_amount);
private:

    const float PLAYER_VEL = 200.0f;
    const float PLAYER_WIDTH = 60;
    const float PLAYER_HEIGHT = 99;
    const float PLAYER_JUMP_AMOUNT = 800.0f;

    //The X and Y offsets
    float _xPos = 10;
    float _yPos = 10;

    //The velocity
    float _xVel = 0;
    float _yVel = 0;

    //flag - the player is on the ground or not
    bool _grounded = false;

    //texture
    LTexture _texture;

    //Collision/position rect
    SDL_FRect _collisionRect = {0,0, PLAYER_WIDTH, PLAYER_HEIGHT};
};

