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
#include "Game.h"


int main(int argc, char* args[])
{
    Game game = Game();
    //Start up SDL and create window
    if (!game.init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        
        if (!game.loadMedia()) //Load media
        {
            printf("Failed to load media!\n");
        }
        else 
        {
            if (!game.initPlayer()) //init the player
            {
                printf("Failed to init player!\n");
            }
            else //run the game
            {
                game.run();
            }
        }
    }
    //Free resources and close SDL
    game.quit();

    return 0;
}
