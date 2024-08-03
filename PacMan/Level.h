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

	void init(size_t tileSize, const std::string& mapFile);

	//getters
	const std::vector<SDL_FRect>& getCollisionWalls() const
	{
		return _collisionWalls;
	}
	const std::vector<int>& getTileMap() const
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

private:
	void initColliders();
	void loadTileMap(const std::string& filename);

public:
	//Where grid starts(top left)
	const int x = 264;
	const int y = 12;

private:

	//The colliders of the level
	std::vector<SDL_FRect> _collisionWalls;

	

	//Tiles for pathfinding
	size_t _tileSize = 0;
	size_t _tileMapWidth = 0;
	size_t _tileMapHeight = 0;
	std::vector<int> _tileMap;

};

