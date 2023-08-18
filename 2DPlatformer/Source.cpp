//Using SDL and standard IO
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
#include "App.h"


int main(int argc, char* args[])
{
    App app = App();
    //Start up SDL and create window
    if (!app.init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //Add the scenes
        app.initScenes();

        //Run the app
        app.run();
    }
    //Free resources and close SDL
    app.quit();

    return 0;
}
