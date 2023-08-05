#pragma once
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GlobalData.h>
#include <Utils.h>
#include <LTexture.h>
#include <Tile.h>

constexpr float PLAYER_VEL = 1.0f;
constexpr float PLAYER_WIDTH = 60;
constexpr float PLAYER_HEIGHT = 99;

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
    void update(std::vector<Tile*>& tiles);


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
private:

    //The X and Y offsets
    float _xPos = 10;
    float _yPos = 10;

    //The velocity
    float _xVel = 0;
    float _yVel = 0;

    //texture
    LTexture _texture;

    //Collision/position rect
    SDL_FRect _collisionRect = {0,0, PLAYER_WIDTH, PLAYER_HEIGHT};
};

