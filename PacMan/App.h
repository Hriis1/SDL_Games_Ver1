#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <memory>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include <Scene.h>
#include <LWindow.h>

class App
{
public:
	App() = default;

	bool init();
	void initScenes();
	void run();
	void quit();
private:
	std::vector<std::unique_ptr<IScene>> _scenes;

	//Custom windows
	LWindow _window;

	int _currentScene = 0;
	bool _quit = false;
	
};

