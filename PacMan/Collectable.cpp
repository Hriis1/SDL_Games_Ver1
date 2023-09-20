#include "Collectable.h"

LTexture Collectable::_smallCollectableSprite;
LTexture Collectable::_bigCollectableSprite;

Collectable::Collectable(float xPos, float yPos, CollectableType type)
	: _xPos(xPos), _yPos(yPos), _type(type)
{
}

bool Collectable::init(SDL_Renderer* renderer, SDL_Window* window)
{
	return false;
}

void Collectable::render(int camX, int camY)
{
}
