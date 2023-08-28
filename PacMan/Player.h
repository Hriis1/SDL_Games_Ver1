#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GlobalData.h>
#include <Utils.h>
#include <LTexture.h>

constexpr float PLAYER_VEL = 1.0f;
constexpr float PLAYER_WIDTH = 16;
constexpr float PLAYER_HEIGHT = 16;
class Player
{
    //This Player class has circilar collision detector
    //The position of the Player is the center of the circle
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
    const Circle<float>& getCollider() const
    {
        return _collisionCircle;
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

    //The X and Y offsets of the Player
    float _xPos = 10;
    float _yPos = 10;

    //The velocity of the Player
    float _xVel = 0;
    float _yVel = 0;

    //Players texture
    LTexture _PlayerTexture;

    //Players collision circle
    Circle<float> _collisionCircle;

};

