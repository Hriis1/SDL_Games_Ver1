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

class App
{
public:
	App() = default;

	void init();
	void run();
private:
	std::vector<IScene*> _scenes;
	
};

