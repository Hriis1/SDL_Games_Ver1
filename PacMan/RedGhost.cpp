#include "RedGhost.h"

RedGhost::RedGhost(float xPos, float yPos, float pathFindInterval)
    : Ghost(xPos, yPos, pathFindInterval)
{
    _type = GhostType::RED;

    //scatter to the top
    _targetScatterTiles.push_back({ 43, 2 }); 
    _targetScatterTiles.push_back({ 4, 6 });
}

std::vector<A_Point> RedGhost::pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts)
{
    //Get the target position
    SDL_Point targetGridPos = { 0, 0 };
    if(_movementMode == GhostMovementMode::Chase) //if the ghost is in chase mode
        targetGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());
    else if (_movementMode == GhostMovementMode::Scatter)
    {
        int currIdx = _targetScatterTileIdx % 2;
        targetGridPos = _targetScatterTiles[currIdx];
    }
    A_Point targetGridAPos = { targetGridPos.x, targetGridPos.y };

    //Get the ghost position
    SDL_Point ghostGridPos = level.getWalkableGridPos(getCenterPos<SDL_Point, int>());
    A_Point ghostGridAPos = { ghostGridPos.x, ghostGridPos.y };

    //The isWalkable function ptr
    auto isWalkable = std::bind(&Level::isWalkable, &level, std::placeholders::_1, std::placeholders::_2);

    //Get the path
    size_t mapWidth = level.getTileMapWidth();
    size_t mapHright = level.getTileMapHeight();
    auto path = aStar(ghostGridAPos, targetGridAPos, mapWidth, isWalkable);

    return path;
}
