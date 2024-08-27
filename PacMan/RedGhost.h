#pragma once
#include "Ghost.h"
class RedGhost : public Ghost
{
public:
	RedGhost(float xPos, float yPos, GhostType type = GhostType::RED);

	std::vector<A_Point> pathFind(const Level& level, const Player& player) override;
};

