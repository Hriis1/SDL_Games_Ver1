#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <SDL/SDL.h>
class Level
{
public:
	Level();

	void init(int tileSize, const std::string& mapFile);

	//getters
	const std::vector<SDL_FRect>& getCollisionWalls() const
	{
		return _collisionWalls;
	}

private:
	void initColliders();
	void loadTileMap(const std::string& filename);

private:

	//The colliders of the level
	std::vector<SDL_FRect> _collisionWalls;

	//Tiles for pathfinding
	int _tileSize = 0;
	int _tileMapWidth = 0;
	int _tileMapHeight = 0;
	std::vector<int> _tileMap;

};

