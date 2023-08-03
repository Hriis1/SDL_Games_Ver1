#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GlobalData.h>
#include <Utils.h>
#include <LTexture.h>
#include <Tile.h>

constexpr float DOT_VEL = 1.0f;
constexpr float DOT_WIDTH = 20;
constexpr float DOT_HEIGHT = 20;
class Dot
{
    //This dot class has circilar collision detector
    //The position of the dot is the center of the circle
public:
    //Initializes the variables
    Dot(SDL_Renderer* renderer, SDL_Window* window);

    //Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);

    //Moves the dot
    void update(std::vector<SDL_FRect>& boxes, std::vector<Circle<float>> circles, Tile* tiles[]);


    //Shows the dot on the screen
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
    //Moves the collision circle relative to the dot's offset
    void shiftColliders();
private:

    //The X and Y offsets of the dot
    float _xPos = 10;
    float _yPos = 10;

    //The velocity of the dot
    float _xVel = 0;
    float _yVel = 0;

    //Dots texture
    LTexture _dotTexture;

    //Dots collision circle
    Circle<float> _collisionCircle;

};

