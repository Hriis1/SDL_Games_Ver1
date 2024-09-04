#include "BlueGhost.h"
#include <cmath>
#include <algorithm>

BlueGhost::BlueGhost(SDL_Renderer* renderer, SDL_Window* window, float xPos, float yPos, float pathFindInterval)
    : Ghost(xPos, yPos, pathFindInterval)
{
    //Init the sprite sheet of the ghost
    _spriteSheet.initRenderer(renderer);
    if (!_spriteSheet.loadFromFile("Assets/Textures/blueGhost.png", window))
    {
        printf("Could not load blue ghost texture\n");
    }

    _type = GhostType::BLUE;
    _fillTileColor.r = 0;
    _fillTileColor.g = 0;
    _fillTileColor.b = 255;

    //scatter to the bot right to left
    _targetScatterTiles.push_back({ 43, 40 });
    _targetScatterTiles.push_back({ 3, 36 });
}

std::vector<A_Point> BlueGhost::pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts)
{
    //Get the target position
    if (_movementMode == GhostMovementMode::Chase) //if the ghost is in chase mode
    {
        //Get the position of the player
        auto futurePlayerGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());

        //Get the direction of the player
        SDL_Point playerDir = { 0, 0 };
        if (player.getDirection() == DIRECTION::LEFT)
            playerDir = { -1, 0 };
        else if (player.getDirection() == DIRECTION::RIGHT)
            playerDir = { 1, 0 };
        else if (player.getDirection() == DIRECTION::UP)
            playerDir = { 0, -1 };
        else
            playerDir = { 0, 1 };

        //Get the target pos of the ghost
        futurePlayerGridPos.x += 3 * playerDir.x;
        futurePlayerGridPos.y += 3 * playerDir.y;

        //Get the position of the red ghost
        SDL_Point redGhostPos = { 0, 0 };
        for (size_t i = 0; i < ghosts.size(); i++)
        {
            if (ghosts[i]->getType() == GhostType::RED)
            {
                redGhostPos = level.getWalkableGridPos(ghosts[i]->getCenterPos<SDL_Point, int>());
                break;
            }
        }

        SDL_Point ghostToFuturePlayerPosDist = { futurePlayerGridPos.x - redGhostPos.x, futurePlayerGridPos.y - redGhostPos.y };
        _targetGridPos.x = std::max(-3, std::min(futurePlayerGridPos.x + ghostToFuturePlayerPosDist.x, (int)level.getTileMapWidth() + 3)); //clamp the values between -3 and the width of the level + 3
        _targetGridPos.y = std::max(-3, std::min(futurePlayerGridPos.y + ghostToFuturePlayerPosDist.y, (int)level.getTileMapHeight() + 3)); //clamp the values between -3 and the height of the level + 3

        //Grt the opisite direction of the dist vector
        int dirX = ghostToFuturePlayerPosDist.x == 0 ? 0 : ghostToFuturePlayerPosDist.x / abs(ghostToFuturePlayerPosDist.x);
        int dirY = ghostToFuturePlayerPosDist.y == 0 ? 0 : ghostToFuturePlayerPosDist.y / abs(ghostToFuturePlayerPosDist.y);
        SDL_Point dir = { dirX, dirY };


        _targetGridPos = level.getClosestWalkableGridPos(_targetGridPos, { -dir.x, -dir.y });

    }
    else if (_movementMode == GhostMovementMode::Scatter)
    {
        int currIdx = _targetScatterTileIdx % 2;
        _targetGridPos = _targetScatterTiles[currIdx];
    }
    A_Point targetGridAPos = { _targetGridPos.x, _targetGridPos.y };

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
