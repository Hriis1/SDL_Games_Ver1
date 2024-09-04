#pragma once
#include "Ghost.h"
class OrangeGhost : public Ghost
{
public:
	OrangeGhost(SDL_Renderer* renderer, SDL_Window* window, float xPos, float yPos, float pathFindInterval);

	std::vector<A_Point> pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts) override;

private:
	const int _distToPlayerThreshold = 12; // how many tiles away should ghost target pacman from
};

