#include "GreenGhost.h"

GreenGhost::GreenGhost(float xPos, float yPos, float pathFindInterval)
    : Ghost(xPos, yPos, pathFindInterval)
{
    _type = GhostType::GREEN;

    //scatter to the top
    _targetScatterTiles.push_back({ 43, 40 });
    _targetScatterTiles.push_back({ 3, 36 });
}

std::vector<A_Point> GreenGhost::pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts)
{
    //Get the target position
    SDL_Point targetGridPos = { 0, 0 };
    if (_movementMode == GhostMovementMode::Chase) //if the ghost is in chase mode
    {
        //Get the position of the player
        auto futurePlayerGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());

        //Get the direction of the player
        SDL_Point dir = { 0, 0 };
        if (player.getDirection() == DIRECTION::LEFT)
            dir = { -1, 0 };
        else if (player.getDirection() == DIRECTION::RIGHT)
            dir = { 1, 0 };
        else if (player.getDirection() == DIRECTION::UP)
            dir = { 0, -1 };
        else
            dir = { 0, 1 };

        //Get the target pos of the ghost
        futurePlayerGridPos.x += 6 * dir.x;
        futurePlayerGridPos.y += 6 * dir.y;
        targetGridPos = level.getClosestWalkableGridPos(futurePlayerGridPos, { dir.x * -1, dir.y * -1 });

    }
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
