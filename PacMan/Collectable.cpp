#include "Collectable.h"

const float Collectable::TEXTURE_SCALE = 2.0f;
const float Collectable::COLLECTABLE_HEIGHT = 16 * Collectable::TEXTURE_SCALE;
const float Collectable::COLLECTABLE_WIDTH = 16 * Collectable::TEXTURE_SCALE;
LTexture Collectable::_smallCollectableSpriteSheet;
LTexture Collectable::_bigCollectableSpriteSheet;
SDL_Rect Collectable::_spriteClips[COLLECTABLE_ANIMATION_FRAMES];

Collectable::Collectable(float xPos, float yPos, CollectableType type)
	: _xPos(xPos), _yPos(yPos), _type(type)
{
}

bool Collectable::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _smallCollectableSpriteSheet.initRenderer(renderer);
    _bigCollectableSpriteSheet.initRenderer(renderer);

    if (!_smallCollectableSpriteSheet.loadFromFile("Assets/Textures/Coin.png", window))
    {
        printf("Could not load small coin texture\n");
        return false;
    }
    if (!_bigCollectableSpriteSheet.loadFromFile("Assets/Textures/BigCoin.png", window))
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
    SDL_Rect* currentClip = &_spriteClips[_animationFrame / 12];

    if (_type == CollectableType::SMALL)
    {
        _smallCollectableSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else //if the type is BIG
    {
        _bigCollectableSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }

    //Go to next frame
    _animationFrame++;

    //Cycle animation
    if (_animationFrame / 12 >= COLLECTABLE_ANIMATION_FRAMES)
    {
        _animationFrame = 0;
    }
}
