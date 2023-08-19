#pragma once
#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include <Scene.h>
#include <GlobalData.h>
#include <LTexture.h>
#include <LWindow.h>
#include <LTimer.h>
#include <Tile.h>

#include "Player.h"

class Level2Scene : public IScene
{
public:
	Level2Scene(LWindow& win, bool& quitFlag);
	~Level2Scene();

	//Inherited from IScene
	bool init() override;
	bool loadMedia() override;
	void handleEvents(SDL_Event& e) override;
	void update() override;
	void draw() override;
	void quit() override;

	//Init player function should be called after the media is loaded
	bool initPlayer();
	void restart();


private:

	//Custom windows
	LWindow& _window;

	//Fonts
	TTF_Font* _font = NULL;
	TTF_Font* _gameOverFont = NULL;
	TTF_Font* _restartFont = NULL;

	//Text colors
	SDL_Color _textColor = { 0, 0, 0, 0xFF };

	//Keeps track of deltaTime
	LTimer _deltaTimer;

	//Textures
	LTexture _bgTexture;

	//Text textures
	LTexture _gameOverText;
	LTexture _gameWonText;
	LTexture _restartText;


	//The camera area
	SDL_FRect camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

	//The tiles
	std::vector<Tile*> _tiles;

	//The playable character (for now)
	Player _player;

	//The players starting position
	const float _xStartingPos = 10;
	const float _yStartingPos = 1500;

	//Quit flag
	bool& _quitFlag;


};



