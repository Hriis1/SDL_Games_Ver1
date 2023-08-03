#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include <GlobalData.h>
#include <LTexture.h>
#include <LWindow.h>
#include <Tile.h>

#include "Dot.h"
class Game
{
public:
	Game();
	~Game();

	bool init();
	bool loadMedia();
	void run();
	void quit();

private:

	//Custom windows
	LWindow _window;

	//Fonts
	TTF_Font* _font = NULL;

	//Text colors
	SDL_Color _textColor = { 0, 0, 0, 0xFF };

	//The tiles
	std::vector<Tile*> _tiles;

	//The playable character (for now)
	Dot _dot;

};

