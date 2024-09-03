#pragma once
#include "Ghost.h"
class BlueGhost : public Ghost
{
public:
	BlueGhost(SDL_Renderer* renderer, SDL_Window* window, float xPos, float yPos, float pathFindInterval);

	std::vector<A_Point> pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts) override;
};
