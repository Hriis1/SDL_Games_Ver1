#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <SDL/SDL.h>
#include <LWindow.h>

class Level
{
public:
	Level();

	void drawTileGrid(const LWindow& _window);

	void fillTile(const LWindow& _window, const SDL_Point& tilePos);

	void init(size_t tileSize, const std::string& mapFile, int scale = 1);

	void printMap();

	//getters
	SDL_Point getGridPos(const SDL_Point& worldPos) const;

	SDL_Point getWorldPos(const SDL_Point& tilePos) const;

	//For player or ghost
	SDL_Point getWalkableGridPos(const SDL_Point& worldPos) const;

	const std::vector<SDL_FRect>& getCollisionWalls() const
	{
		return _collisionWalls;
	}
	const std::vector<int>& getTileMap() const
	{
		return _tileMap;
	}
	std::vector<int> getTileMapCopy() const
	{
		return _tileMap;
	}
	size_t getTileSize() const
	{
		return _tileSize;
	}
	size_t getTileMapWidth() const
	{
		return _tileMapWidth;
	}
	size_t getTileMapHeight() const
	{
		return _tileMapHeight;
	}
	int getTile(size_t x, size_t y) const
	{
		int pos = _tileMapWidth * y + x;
		if (_tileMap.size() <= pos)
		{
			return -1;
		}
		
		return _tileMap[pos];

	}
	bool isWalkable(int x, int y) const 
	{
		if (x < 0 || y < 0 || x >= _tileMapWidth || y >= _tileMapHeight) 
			return false;

		return _tileMap[y * _tileMapWidth + x] == 0;
	}

private:
	void initColliders();
	void loadTileMap(const std::string& filename, int scale = 1);

public:
	//Where grid starts(top left)
	const int x = 264;
	const int y = 12;

private:

	//The colliders of the level
	std::vector<SDL_FRect> _collisionWalls;

	

	//Tiles for pathfinding
	float _tileSize = 0;
	size_t _tileMapWidth = 0;
	size_t _tileMapHeight = 0;
	std::vector<int> _tileMap;

};

