#include "Collectable.h"

LTexture Collectable::_smallCollectableSprite;
LTexture Collectable::_bigCollectableSprite;

Collectable::Collectable(float xPos, float yPos, CollectableType type)
	: _xPos(xPos), _yPos(yPos), _type(type)
{
}

bool Collectable::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _smallCollectableSprite.initRenderer(renderer);
    _bigCollectableSprite.initRenderer(renderer);

    if (!_smallCollectableSprite.loadFromFile("Assets/Textures/Coin.png", window))
    {
        printf("Could not load small coin texture\n");
        return false;
    }
    if (!_bigCollectableSprite.loadFromFile("Assets/Textures/BigCoin.png", window))
    {
        printf("Could not load small coin texture\n");
        return false;
    }

    //init spriteclips
    int textureWidth = COLLECTABLE_WIDTH / TEXTURE_SCALE;
    int textureHeight = COLLECTABLE_HEIGHT / TEXTURE_SCALE;
    _spriteClips[0] = { 0,0, textureWidth, textureHeight };
    _spriteClips[1] = { textureWidth,0, textureWidth, textureHeight };
    _spriteClips[2] = { textureWidth * 2,0, textureWidth, textureHeight };
    _spriteClips[3] = { textureWidth * 3,0, textureWidth, textureHeight };
    _spriteClips[4] = { textureWidth * 4,0, textureWidth, textureHeight };
    _spriteClips[5] = { textureWidth * 5,0, textureWidth, textureHeight };
    _spriteClips[6] = { textureWidth * 6,0, textureWidth, textureHeight };
    _spriteClips[7] = { textureWidth * 7,0, textureWidth, textureHeight };

    return true;
}

void Collectable::render(int camX, int camY)
{
}
