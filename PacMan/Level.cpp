#include "Level.h"
#include <iostream>

Level::Level()
{
}

void Level::drawTileGrid(const LWindow& _window)
{
    //Horizontal lines
    for (int i = x; i <= x + _tileMapWidth * _tileSize; i += _tileSize)
    {
        SDL_RenderDrawLine(_window.renderer, i, y, i, y + _tileMapHeight * _tileSize);
    }

    //Vertical lines
    for (int i = y; i <= y + _tileMapHeight * _tileSize; i += _tileSize)
    {
        SDL_RenderDrawLine(_window.renderer, x, i, x + _tileMapWidth * _tileSize, i);
    }
}

void Level::fillTile(const LWindow& _window, const SDL_Point& tilePos) const
{
    SDL_FRect tile = { x + tilePos.x * _tileSize, y + tilePos.y * _tileSize, _tileSize, _tileSize };

    SDL_RenderFillRectF(_window.renderer, &tile);
}

void Level::init(size_t tileSize, const std::string& mapFile, int scale)
{
    //Colliders
    initColliders();

    //Tiles
    _tileSize = tileSize;
    loadTileMap(mapFile, scale);
}

void Level::printMap()
{
    for (size_t i = 0; i < _tileMapHeight; i++)
    {
        for (size_t j = 0; j < _tileMapWidth; j++)
        {
            std::cout << _tileMap[i * _tileMapWidth + j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

SDL_Point Level::getGridPos(const SDL_Point& worldPos) const
{
    int tileX = (worldPos.x - x) / _tileSize;
    int tileY = (worldPos.y - y) / _tileSize;

    return SDL_Point{ tileX, tileY };
}

SDL_Point Level::getWorldPos(const SDL_Point& tilePos) const
{
    int worldX = x + (tilePos.x * _tileSize);
    int worldY = y + (tilePos.y * _tileSize);

    return SDL_Point{ worldX, worldY};
}

SDL_Point Level::getWalkableGridPos(const SDL_Point& worldPos) const
{
    int tileX = (worldPos.x - x) / _tileSize;
    int tileY = (worldPos.y - y) / _tileSize;

    //If position is not walkable take right then down then down right
    if(isWalkable(tileX, tileY))
        return SDL_Point{ tileX, tileY };
    if (isWalkable(tileX + 1, tileY))
        return SDL_Point{ tileX + 1, tileY };
    if (isWalkable(tileX, tileY + 1))
        return SDL_Point{ tileX, tileY + 1 };
    if(isWalkable(tileX + 1, tileY + 1))
        return SDL_Point{ tileX + 1, tileY + 1 };
    if (isWalkable(tileX - 1, tileY))
        return SDL_Point{ tileX - 1, tileY };
    if (isWalkable(tileX, tileY - 1))
        return SDL_Point{ tileX, tileY - 1 };
    if (isWalkable(tileX - 1, tileY - 1))
        return SDL_Point{ tileX - 1, tileY - 1 };

    return SDL_Point{ tileX, tileY };
}

SDL_Point Level::getClosestWalkableGridPos(const SDL_Point gridPos, SDL_Point dir) const
{
    if (!isWalkable(gridPos.x, gridPos.y))
    {
        return getClosestWalkableGridPos({ gridPos.x + dir.x, gridPos.y + dir.y }, dir);
    }

    return gridPos;
}

void Level::initColliders()
{

    if (_collisionWalls.size() == 0) //if the collision walls were not yet inited yet
    {
        //Outside walls
        _collisionWalls.emplace_back(SDL_FRect{ 284, 18, 712, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 996, 18, 17, 680 });
        _collisionWalls.emplace_back(SDL_FRect{ 267, 18, 17, 680 });

        _collisionWalls.emplace_back(SDL_FRect{ 284, 260, 121, 15 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 260, 121, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 388, 275, 17, 44 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 275, 17, 44 });

        _collisionWalls.emplace_back(SDL_FRect{ 284, 319, 121, 15 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 319, 121, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 284, 382, 121, 15 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 382, 121, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 388, 395, 17, 45 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 395, 17, 45 });

        _collisionWalls.emplace_back(SDL_FRect{ 284, 440, 121, 15 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 440, 121, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 284, 684, 712, 15 });

        //Inside walls
        _collisionWalls.emplace_back(SDL_FRect{ 349, 78, 582, 15 });
        _collisionWalls.emplace_back(SDL_FRect{ 349, 623, 582, 15 });

        _collisionWalls.emplace_back(SDL_FRect{ 632, 93, 17, 59 });
        _collisionWalls.emplace_back(SDL_FRect{ 632, 564, 17, 59 });

        _collisionWalls.emplace_back(SDL_FRect{ 349, 140, 96, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 835, 140, 96, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 349, 563, 96, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 835, 563, 96, 13 });

        _collisionWalls.emplace_back(SDL_FRect{ 510, 140, 59, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 713, 140, 59, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 510, 563, 59, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 713, 563, 59, 13 });

        _collisionWalls.emplace_back(SDL_FRect{ 389, 153, 17, 60 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 153, 17, 60 });
        _collisionWalls.emplace_back(SDL_FRect{ 389, 503, 17, 60 });
        _collisionWalls.emplace_back(SDL_FRect{ 875, 503, 17, 60 });

        _collisionWalls.emplace_back(SDL_FRect{ 470, 200, 17, 134 });
        _collisionWalls.emplace_back(SDL_FRect{ 794, 200, 17, 134 });
        _collisionWalls.emplace_back(SDL_FRect{ 470, 382, 17, 134 });
        _collisionWalls.emplace_back(SDL_FRect{ 794, 382, 17, 134 });

        _collisionWalls.emplace_back(SDL_FRect{ 551, 201, 180, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 551, 504, 180, 13 });

        _collisionWalls.emplace_back(SDL_FRect{ 632, 214, 17, 59 });
        _collisionWalls.emplace_back(SDL_FRect{ 632, 444, 17, 59 });

        _collisionWalls.emplace_back(SDL_FRect{ 551, 261, 17, 73 });
        _collisionWalls.emplace_back(SDL_FRect{ 713, 261, 17, 73 });
        _collisionWalls.emplace_back(SDL_FRect{ 551, 382, 17, 73 });
        _collisionWalls.emplace_back(SDL_FRect{ 713, 382, 17, 73 });

        _collisionWalls.emplace_back(SDL_FRect{ 566, 321, 43, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 672, 321, 43, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 566, 382, 43, 13 });
        _collisionWalls.emplace_back(SDL_FRect{ 672, 382, 43, 13 });
    }
}

void Level::loadTileMap(const std::string& filename, int scale) {

    if (_tileMap.size() == 0) //check if the map has already been inited
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Unable to open file";
            return;
        }

        std::string line;
        // Read the first line to get width and height
        if (std::getline(file, line)) {
            std::istringstream ss(line);
            char delimiter;
            ss >> _tileMapWidth >> delimiter >> _tileMapHeight;
        }

        //Scale the width and height
        _tileMapWidth *= scale;
        _tileMapHeight *= scale;

        //scale the tile size
        _tileSize /= (float)scale;

        // Read the rest of the file to get the tile map
        _tileMap.resize(_tileMapWidth * _tileMapHeight);
        size_t idx = 0;
        while (std::getline(file, line)) {
            for (size_t y = 0; y < scale; y++) // scale the character vertically 
            {
                for (char ch : line) {
                    for (size_t x = 0; x < scale; x++) // scale the character horizontally 
                    {
                        _tileMap[idx] = (ch - '0'); // Convert char to int
                        idx++;
                    }
                }
            }
            
        }

        file.close();
    }
}
