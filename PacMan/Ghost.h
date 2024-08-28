#pragma once
#include <vector>
#include <functional>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <LTimer.h>
#include <LTexture.h>
#include <A_Star.h>

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

enum class GhostMovementMode
{
    Chase = -1, Scatter =1
};

class Ghost
{
    //This Ghost class has rect collision detector
public:

public:
    //Initializes the variables
    Ghost(float xPos, float yPos, float pathFindInterval);

    //Handles ghost input(debugging)
    void handleEvent(SDL_Event& e, const Level& level, const Player& player);

    //update the ghost
    void update(float deltaTime, const Level& level, const Player& player);

    //Shows the Ghost on the screen
    void render(int camX, int camY);

    //getters
    const SDL_FRect& getCollider() const
    {
        return _collisionRect;
    }

    template<typename PointT, typename VarT>
    PointT getCenterPos() const
    {
        return PointT{ (VarT)(_xPos + GHOST_WIDTH / 2), (VarT)(_yPos + GHOST_HEIGHT / 2) };
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

    static float GET_WIDTH()
    {
        return GHOST_WIDTH;
    }
    static float GET_HEIGHT()
    {
        return GHOST_HEIGHT;
    }

public:
    //Pathfinding algoritm
    virtual std::vector<A_Point> pathFind(const Level& level, const Player& player) = 0;

protected:
    void handleMovementMode();
    void ghostMove(float deltaTime, const Level& level, int pathPosIdx);
    void chasePlayer(float deltaTime, const Player& player, const std::vector<SDL_FRect>& level);
    void printPath(const Level& level, const std::vector<A_Point>& path);

    //Moves the collision rect relative to the Player's offset
    void shiftColliders();

protected:

    //The X and Y offsets of the Ghost
    float _xPos = 0;
    float _yPos = 0;

    //Reverse the y dir when you cant go ydir and xdir
    int _yDirReversal = 1;

    //Dir that the ghost should move to
    SDL_Point _ghostPrevMovementDir = { 0, 0 };
    SDL_Point _ghostRecentMovementDirs = { 0, 0 };
    SDL_Point _ghostMovementDir = {0, 0};
    

    //The type of the Ghost
    GhostType _type;

    //Ghosts collision circle
    SDL_FRect _collisionRect = { 0,0, GHOST_WIDTH, GHOST_HEIGHT };

    //animation frames
    int _animationFrame = 0;

    //Stuff for pathfinding
    LTimer _pathFindTimer;
    float _pathFindInterval = 0.0f;
    std::vector<A_Point> _pathToFollow;
    LTimer _movementModeTimer;
    GhostMovementMode _movementMode = GhostMovementMode::Scatter;
    SDL_Point _targetScatterTile = { 1, 1 };
    int _movementModeIdx = 0;

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

    //Intervals for movement mode
    static std::vector<int> _movementModeIntervals;


};





