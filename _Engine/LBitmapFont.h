#pragma once
#include <string>
#include <SDL/SDL.h>

#include "LTexture.h"

class LBitmapFont
{
public:
    //The default constructor
    LBitmapFont();

    //Destructor
    ~LBitmapFont();

    //Generates the font
    bool buildFont(const std::string& path, SDL_Window* window, SDL_Renderer* renderer);

    //Deallocates font
    void free();

    //Shows the text
    void renderText(int x, int y, const std::string& text);

private:
    //The font texture
    LTexture _fontTexture;

    //The individual characters in the surface
    SDL_Rect _chars[256];

    //Spacing Variables
    int _newLine = 0;
    int _space = 0;
};

