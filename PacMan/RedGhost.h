#pragma once
#include "Ghost.h"
class RedGhost : public Ghost
{
public:
	RedGhost(float xPos, float yPos, float pathFindInterval);

	std::vector<A_Point> pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts) override;
};

