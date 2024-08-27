#include "RedGhost.h"

RedGhost::RedGhost(float xPos, float yPos, GhostType type)
    : Ghost(xPos, yPos, type)
{
}

std::vector<A_Point> RedGhost::pathFind(const Level& level, const Player& player)
{
    //Get the positions of the player and ghost
    SDL_Point playerGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());
    A_Point playerGridAPos = { playerGridPos.x, playerGridPos.y };
    SDL_Point ghostGridPos = level.getWalkableGridPos(getCenterPos<SDL_Point, int>());
    A_Point ghostGridAPos = { ghostGridPos.x, ghostGridPos.y };

    //The isWalkable function ptr
    auto isWalkable = std::bind(&Level::isWalkable, &level, std::placeholders::_1, std::placeholders::_2);

    //Get the path
    size_t mapWidth = level.getTileMapWidth();
    size_t mapHright = level.getTileMapHeight();
    auto path = aStar(ghostGridAPos, playerGridAPos, mapWidth, isWalkable);

    return path;
}
