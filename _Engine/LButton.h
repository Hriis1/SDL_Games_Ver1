#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string>

#include "LTexture.h"

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;

class LButton
{
public:
    //Initializes internal variables
    LButton();

    //Handles mouse event
    void handleEvent(SDL_Event* e);

    //Shows button sprite
    void render();

    //setters
   
    //Sets top left position
    void setPosition(int x, int y);

    //static
    //init functiin for all buttons
    static bool init(SDL_Renderer* renderer, SDL_Window* window);

private:
    //Top left position
    SDL_Point _position = { 0,0 };

    //Currently used global sprite
    LButtonSprite _currentSprite;

    //static
    //sprites used for all buttons
    static bool _spritesInited;
    static LTexture _buttonSpriteSheetTexture;
    static SDL_Rect _spriteClips[BUTTON_SPRITE_TOTAL];
};

