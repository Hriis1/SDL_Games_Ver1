#pragma once
#include <vector>
#include <string>
#include <fstream>

#include <SDL/SDL.h>
class Level
{
public:
	Level();

	void init();

	//getters
	const std::vector<SDL_FRect>& getCollisionWalls() const
	{
		return _collisionWalls;
	}

private:
	void initColliders();

private:

	//The colliders of the level
	std::vector<SDL_FRect> _collisionWalls;

};

