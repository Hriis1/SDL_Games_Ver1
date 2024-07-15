#pragma once
#include <iostream>
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

#include "Flags.h"
#include "Player.h"
#include "Collectable.h"
#include "Ghost.h"

class WinScene : public IScene
{
public:
	WinScene(LWindow& win, bool& quitFlag);
	~WinScene();

	//Inherited from IScene
	bool init() override;
	bool loadMedia() override;
	void handleEvents(SDL_Event& e) override;
	void update() override;
	void draw() override;
	void quit() override;

	void restart();

private:

	//Resets all objects to their original state
	void reset();

private:

	//Custom windows
	LWindow& _window;

	//Fonts
	TTF_Font* _font = NULL;

	//Text colors
	SDL_Color _textColor = { 0, 0, 0, 0xFF };

	//Keeps track of deltaTime
	LTimer _deltaTimer;

	//Textures
	LTexture _bgTexture;

	//Text textures
	LTexture _gameWonText;

	//The camera area
	SDL_FRect _camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };


	//Quit flag
	bool& _quitFlag;

	//Flag for things only developers should see
	bool _developerVision = DEVELOPER_MODE;

	


};

