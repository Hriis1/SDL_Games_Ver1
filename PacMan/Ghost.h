#pragma once
#pragma once
#pragma once
#include <vector>
#include <functional>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <LTexture.h>

#include "PacManData.h"
#include "Player.h"
#include "Level.h"

const int Ghost_ANIMATION_FRAMES = 8;

enum class GhostType
{
    RED = 0, GREEN, BLUE, ORANGE, YELLOW
};

enum GhostMovementDir
{
    GHOST_RIGHT = 1, GHOST_LEFT = 10,
    GHOST_TOP = 2, GHOST_BOTTOM = 20
};

class Ghost
{
    //This Ghost class has rect collision detector
public:

public:
    //Initializes the variables
    Ghost(float xPos, float yPos, GhostType type = GhostType::RED);

    //update the ghost
    void update(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level = {});

    //Shows the Ghost on the screen
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

    GhostType getType() const
    {
        return _type;
    }

    //static
    //Init the Ghost textures
    static bool init(SDL_Renderer* renderer, SDL_Window* window);

public:
    //Pathfinding and chasing the player
    void pathFindToPlayerAStar(const Level& level, const Player& player);
private:
    void chasePlayer(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level);
    void chasePlayerAStar(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level);

    //Moves the collision rect relative to the Player's offset
    void shiftColliders();

private:

    //The X and Y offsets of the Ghost
    float _xPos = 0;
    float _yPos = 0;

    //Reverse the y dir when you cant go ydir and xdir
    int _yDirReversal = 1;

    //The type of the Ghost
    GhostType _type;

    //Ghosts collision circle
    SDL_FRect _collisionRect = { 0,0, GHOST_WIDTH, GHOST_HEIGHT };

    //animation frames
    int _animationFrame = 0;

    //static
    //consts
    static const float TEXTURE_SCALE;
    static const float GHOST_WIDTH;
    static const float GHOST_HEIGHT;
    static const float GHOST_VEL;

    //sprite clips
    static SDL_Rect _spriteClips[Ghost_ANIMATION_FRAMES];

    //Ghosts texture
    static LTexture _redGhostSpriteSheet;
    static LTexture _greenGhostSpriteSheet;
    static LTexture _blueGhostSpriteSheet;
    static LTexture _orangeGhostSpriteSheet;
    static LTexture _yellowGhostSpriteSheet;


};





