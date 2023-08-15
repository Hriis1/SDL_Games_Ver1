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
    bool init(SDL_Renderer* renderer, SDL_Window* window, TTF_Font* font );

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    //Moves the dot
    //If the function takes a deltaTime the player will be updated independant of framerate 
    void update(std::vector<Tile*>& tiles, float gravity = 0.0f, float deltaTime = 1.0f);

    //Shows the dot on the screen
    void render(int camX, int camY);

    //Resets the player with a new position
    void reset(float xPos, float yPos);

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

    float getWidth() const
    {
        return PLAYER_WIDTH;
    }

    float getHeight() const
    {
        return PLAYER_HEIGHT;
    }

private:
    //Updates the _currentTexture ptr based on the state of the player
    void updateTexture();

    //Moves the collision rect relative to the offset
    void shiftColliders();
    
    //Charges the jump amount
    void chargeJump(float deltaTime);

    //jump :)
    void jump();

private:

    //consts
    const float PLAYER_VEL = 200.0f;
    const float PLAYER_WIDTH = 60;
    const float PLAYER_HEIGHT = 99;
    const float MAX_JUMP_VELICTY = 900.0f;
    const float JUMP_CHARGE = 700.0f;
    const float JUMP_X_VEL = 400.0f;

    //The X and Y offsets
    float _xPos = 0;
    float _yPos = 0;

    //The velocity
    float _walkingVel = 0.0f;
    float _jumpingVel = 0.0f;
    float _xVel = 0;
    float _yVel = 0;
    

    float _jumpVelocity = 0.0f;

    //flags
    bool _grounded = false;
    bool _facingRight = true;
    bool _chargingJump = false;

    //textures
    LTexture* _currentTexture = NULL;
    LTexture _textureStandingStill;
    LTexture _textureMoving;
    LTexture _chargingJumpTexture;

    //Collision/position rect
    SDL_FRect _collisionRect = {0,0, PLAYER_WIDTH, PLAYER_HEIGHT};
};

