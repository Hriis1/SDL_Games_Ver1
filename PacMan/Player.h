#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <LTexture.h>

#include "PacManData.h"
#include "Collectable.h"

const int PLAYER_ANIMATION_FRAMES = 8;

enum PLAYER_MOVEMENT_FLAGS
{
    LEFT = 1, RIGHT = 2, UP = 4, DOWN = 8
};

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
    void update(float  deltaTime, float& score, std::vector<Collectable>& collectables, const std::vector<SDL_FRect>& level = {});

    //reset the player to init condition
    void reset();


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
    //Moves the collision rect relative to the Player's offset
    void shiftColliders();

    void updateDirection();
private:

    //consts
    const float TEXTURE_SCALE = 2.0f;
    const float PLAYER_VEL = 160.0f;
    const float PLAYER_WIDTH = 16 * TEXTURE_SCALE;
    const float PLAYER_HEIGHT = 16 * TEXTURE_SCALE;
    const float X_START_POS = 300.0f;
    const float Y_START_POS = 100.0f;
    

    //The X and Y offsets of the Player
    float _xPos = X_START_POS;
    float _yPos = Y_START_POS;

    //The velocity of the Player
    float _xVel = 0;
    float _yVel = 0;

    //flag for player movement
    int _playerMovementFlag = 0;

    //The direction the player is facing
    DIRECTION _direction = DIRECTION::RIGHT;

    //Players collision circle
    SDL_FRect _collisionRect = { 0,0, PLAYER_WIDTH, PLAYER_HEIGHT };

    //Players texture
    LTexture _playerSpriteSheet;
  
    //Spriteclips
    int _animationFrame = 0;
    SDL_Rect _spriteClips[PLAYER_ANIMATION_FRAMES];

};

