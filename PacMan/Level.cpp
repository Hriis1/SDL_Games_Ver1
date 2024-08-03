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
        SDL_RenderDrawLine(_window.renderer, i, 12, i, 705);
    }

    //Vertical lines
    for (int i = y; i <= y + _tileMapHeight * _tileSize; i += _tileSize)
    {
        SDL_RenderDrawLine(_window.renderer, 264, i, 1020, i);
    }
}

void Level::fillPlayerTile(const LWindow& _window, const Player& player)
{
    int playerX = player.getXPos();
    int playerY = player.getYPos();

}

void Level::init(size_t tileSize, const std::string& mapFile)
{
    //Colliders
    initColliders();

    //Tiles
    _tileSize = tileSize;
    loadTileMap(mapFile);
}

SDL_Point Level::getPlayerGridPos(const Player& player)
{
    int playerX = player.getXPos();
    int playerY = player.getYPos();

    int tileX = (playerX - x) / (int)_tileSize;
    int tileY = (playerY - y) / (int)_tileSize;

    return SDL_Point{ tileX, tileY };
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

void Level::loadTileMap(const std::string& filename) {

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

        // Read the rest of the file to get the tile map
        _tileMap.resize(_tileMapWidth * _tileMapHeight);
        size_t idx = 0;
        while (std::getline(file, line)) {
            for (char ch : line) {
                _tileMap[idx] = (ch - '0'); // Convert char to int
                idx++;
            }
        }

        file.close();
    }
}
