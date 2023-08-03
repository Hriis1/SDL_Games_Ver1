#include "LButton.h"

bool LButton::_spritesInited = false;
LTexture LButton::_buttonSpriteSheetTexture;
SDL_Rect LButton::_spriteClips[BUTTON_SPRITE_TOTAL];

LButton::LButton()
	: _currentSprite(LButtonSprite::BUTTON_SPRITE_MOUSE_OUT)
{
}

void LButton::handleEvent(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Mouse is inside the button
		if (x >= _position.x && x <= _position.x + BUTTON_WIDTH &&
			y >= _position.y && y <= _position.y + BUTTON_HEIGHT)
		{
            //Set mouse sprite
            switch (e->type)
            {
            case SDL_MOUSEMOTION:
                _currentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

            case SDL_MOUSEBUTTONDOWN:
                _currentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                _currentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_UP;
                break;
            }
		}
        else //Mouse is outside the button
        {
            _currentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
        }
	}
}

void LButton::render()
{
    //Show current button sprite
    _buttonSpriteSheetTexture.render(_position.x, _position.y, &_spriteClips[_currentSprite]);
}

void LButton::setPosition(int x, int y)
{
	_position.x = x;
	_position.y = y;
}


bool LButton::init(SDL_Renderer* renderer, SDL_Window* window)
{
    if (!_spritesInited)
    {
        _spritesInited = true;

        //Load sprites
        _buttonSpriteSheetTexture.initRenderer(renderer);
        if (!_buttonSpriteSheetTexture.loadFromFile("D:/programing/C++ SDL lazy foo tutorials/sdl_tutorials/_Engine/button.png", window))
        {
            printf("Failed to load button sprite texture!\n");
            return false;
        }

        //Set sprites
        for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
        {
            _spriteClips[i].x = 0;
            _spriteClips[i].y = i * 200;
            _spriteClips[i].w = BUTTON_WIDTH;
            _spriteClips[i].h = BUTTON_HEIGHT;
        }
    }

    return true;
}
