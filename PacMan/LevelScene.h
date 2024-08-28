#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include <Scene.h>
#include <GlobalData.h>
#include <LTexture.h>
#include <LWindow.h>
#include <LTimer.h>

#include "Level.h"
#include "Flags.h"
#include "Player.h"
#include "Collectable.h"
#include "Ghost.h"
#include "RedGhost.h"
#include "GreenGhost.h"

class LevelScene : public IScene
{
public:
	LevelScene(LWindow& win, bool& quitFlag);
	~LevelScene();

	//Inherited from IScene
	bool init() override;
	bool loadMedia() override;
	void handleEvents(SDL_Event& e) override;
	void update() override;
	void draw() override;
	void quit() override;

	void restart();

private:
	void initCollectables();
	void initGhosts();

	//Resets all objects to their original state
	void reset();
	
private:

	//Custom windows
	LWindow& _window;

	//Fonts

	//Text colors
	SDL_Color _textColor = { 0, 0, 0, 0xFF };

	//Keeps track of deltaTime
	LTimer _deltaTimer;

	//Textures
	LTexture _bgTexture;

	//Text textures

	//The camera area
	SDL_FRect _camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Player
	Player _player;

	//Level data
	Level _level;

	//Coins
	std::vector<Collectable> _coins;

	//ghosts
	std::vector<std::unique_ptr<Ghost>> _ghosts;

	//Quit flag
	bool& _quitFlag;

	//the current score
	float _score = 0.0f;

	//Flag for things only developers should see
	bool _developerVision = false;
	

};

