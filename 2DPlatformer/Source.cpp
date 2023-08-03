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


//Custom windows
LWindow gWindow;

//Fonts
TTF_Font* gFont = NULL;

//Global text colors
SDL_Color gTextColor = { 0, 0, 0, 0xFF };

//The tiles
std::vector<Tile*>tiles;

//Starts up SDL and creates window
bool init()
{
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }  
    //Create windows and create renderers
    if (!gWindow.init())
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Initialize PNG  and JPG loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    //Initialize SDL_mixer
    /*if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }*/

    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    if (!Tile::initTiles(gWindow.renderer, gWindow.getSDLWindow()))
    {
        printf("Could not init tiles\n");
        return false;
    }

    return true;
}

//Loads media
bool loadMedia()
{
    //Load fonts
    gFont = TTF_OpenFont("../_Engine/fonts/lazy.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    //Load textures

    //Load tiles
    if (!Tile::loadTiles("tileMap.map", tiles))
    {
        printf("Failed to load tile set!\n");
        return false;
    }

       return true;
}

//Frees media and shuts down SDL
void close()
{
    //Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    gWindow.free();

    //Quit SDL systems
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    //Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
           //The camera area
            SDL_Rect camera = { 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT };

            Dot dot = Dot();
            dot.init(gWindow.renderer, gWindow.getSDLWindow());
            std::vector<SDL_FRect> boxes;
            std::vector<Circle<float>> circles;

            //Current input point
            int currentData = 0;

            //Start the game loop and poll events
            SDL_Event e;
            bool quit = false;

            while (quit == false) 
            {
                while (SDL_PollEvent(&e)) 
                {
                    if (e.type == SDL_QUIT) 
                        quit = true;

                    //Handle window events
                    gWindow.handleEvent(e);
                   
                    //Handle input for the dot
                    dot.handleEvent(e);

                    //Special key input
                    if (e.type == SDL_KEYDOWN)
                    {
                       
                    }
                } 

                //Only draw when not minimized
                if (!gWindow.isMinimized())
                {
                    //Update
                    dot.update(boxes, circles, tiles);

                    //Center the camera over the dot
                    camera.x = (dot.getXPos()) - SCREEN_WIDTH / 2;
                    camera.y = (dot.getYPos()) - SCREEN_HEIGHT / 2;

                    //Keep the camera in bounds
                    if (camera.x < 0)
                    {
                        camera.x = 0;
                    }
                    if (camera.y < 0)
                    {
                        camera.y = 0;
                    }
                    if (camera.x > LEVEL_WIDTH - camera.w)
                    {
                        camera.x = LEVEL_WIDTH - camera.w;
                    }
                    if (camera.y > LEVEL_HEIGHT - camera.h)
                    {
                        camera.y = LEVEL_HEIGHT - camera.h;
                    }

                    //Clear screen
                    SDL_SetRenderDrawColor(gWindow.renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gWindow.renderer);

                    //Render level
                    for (int i = 0; i < TOTAL_TILES; ++i)
                    {
                        tiles[i]->render(camera);
                    }

                    //Render the textures
                    dot.render(camera.x, camera.y);

                    //Update screen
                    SDL_RenderPresent(gWindow.renderer);
                }
            }
        }
    }
    //Free resources and close SDL
    close();

    return 0;
}
