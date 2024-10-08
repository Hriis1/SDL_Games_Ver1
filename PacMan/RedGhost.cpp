#include "RedGhost.h"

RedGhost::RedGhost(SDL_Renderer* renderer, SDL_Window* window, float xPos, float yPos, float pathFindInterval)
    : Ghost(xPos, yPos, pathFindInterval)
{
    //Init the sprite sheet of the ghost
    _spriteSheet.initRenderer(renderer);
    if (!_spriteSheet.loadFromFile("Assets/Textures/redGhost.png", window))
    {
        printf("Could not load red ghost texture\n");
    }

    _type = GhostType::RED;
    _fillTileColor.r = 255;
    _fillTileColor.g = 0;
    _fillTileColor.b = 0;

    //scatter to the top right to left
    _targetScatterTiles.push_back({ 43, 2 }); 
    _targetScatterTiles.push_back({ 3, 6 });
}

std::vector<A_Point> RedGhost::pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts)
{
    //Get the target position
    if(_movementMode == GhostMovementMode::Chase) //if the ghost is in chase mode
        _targetGridPos = level.getWalkableGridPos(player.getCenterPos<SDL_Point, int>());
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
