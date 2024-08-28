#pragma once
#include "Ghost.h"
class GreenGhost : public Ghost
{
public:
	GreenGhost(float xPos, float yPos, float pathFindInterval);

	std::vector<A_Point> pathFind(const Level& level, const Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts) override;
};

