#pragma once
#include <SDL/SDL.h>
#include <vector>
#include <fstream>

#include "GlobalData.h"
#include "LTexture.h"

//Tile constants
const static int TILE_WIDTH = 80;
const static int TILE_HEIGHT = 80;
const static int TOTAL_TILES = 192;

//The different tile sprites
static enum TILE_TYPE
{
    TILE_RED = 0, TILE_GREEN, TILE_BLUE, TILE_CENTER, TILE_TOP, TILE_TOPRIGHT, TILE_RIGHT,
    TILE_BOTTOMRIGHT, TILE_BOTTOM, TILE_BOTTOMLEFT, TILE_LEFT, TILE_TOPLEFT, TOTAL_TILE_SPRITES
};

class Tile
{
public:
    //Initializes position and type
    Tile(int x, int y, TILE_TYPE tileType);

    //Shows the tile
    void render(SDL_Rect& camera);

    //getters
    SDL_Rect getBox() const
    {
        return _box;
    }

    TILE_TYPE getType() const
    {
        return _type;
    }

    //static
    static bool initTiles(SDL_Renderer* renderer, SDL_Window* window)
    {
        if (!_inited)
        {
            _tilesSheet.initRenderer(renderer);

            if (!_tilesSheet.loadFromFile("../_Engine/tiles/tiles.png", window))
                return false;

            //init _spriteClips
            _spriteClips[TILE_RED].x = 0;
            _spriteClips[TILE_RED].y = 0;
            _spriteClips[TILE_RED].w = TILE_WIDTH;
            _spriteClips[TILE_RED].h = TILE_HEIGHT;

            _spriteClips[TILE_GREEN].x = 0;
            _spriteClips[TILE_GREEN].y = 80;
            _spriteClips[TILE_GREEN].w = TILE_WIDTH;
            _spriteClips[TILE_GREEN].h = TILE_HEIGHT;

            _spriteClips[TILE_BLUE].x = 0;
            _spriteClips[TILE_BLUE].y = 160;
            _spriteClips[TILE_BLUE].w = TILE_WIDTH;
            _spriteClips[TILE_BLUE].h = TILE_HEIGHT;

            _spriteClips[TILE_TOPLEFT].x = 80;
            _spriteClips[TILE_TOPLEFT].y = 0;
            _spriteClips[TILE_TOPLEFT].w = TILE_WIDTH;
            _spriteClips[TILE_TOPLEFT].h = TILE_HEIGHT;

            _spriteClips[TILE_LEFT].x = 80;
            _spriteClips[TILE_LEFT].y = 80;
            _spriteClips[TILE_LEFT].w = TILE_WIDTH;
            _spriteClips[TILE_LEFT].h = TILE_HEIGHT;

            _spriteClips[TILE_BOTTOMLEFT].x = 80;
            _spriteClips[TILE_BOTTOMLEFT].y = 160;
            _spriteClips[TILE_BOTTOMLEFT].w = TILE_WIDTH;
            _spriteClips[TILE_BOTTOMLEFT].h = TILE_HEIGHT;

            _spriteClips[TILE_TOP].x = 160;
            _spriteClips[TILE_TOP].y = 0;
            _spriteClips[TILE_TOP].w = TILE_WIDTH;
            _spriteClips[TILE_TOP].h = TILE_HEIGHT;

            _spriteClips[TILE_CENTER].x = 160;
            _spriteClips[TILE_CENTER].y = 80;
            _spriteClips[TILE_CENTER].w = TILE_WIDTH;
            _spriteClips[TILE_CENTER].h = TILE_HEIGHT;

            _spriteClips[TILE_BOTTOM].x = 160;
            _spriteClips[TILE_BOTTOM].y = 160;
            _spriteClips[TILE_BOTTOM].w = TILE_WIDTH;
            _spriteClips[TILE_BOTTOM].h = TILE_HEIGHT;

            _spriteClips[TILE_TOPRIGHT].x = 240;
            _spriteClips[TILE_TOPRIGHT].y = 0;
            _spriteClips[TILE_TOPRIGHT].w = TILE_WIDTH;
            _spriteClips[TILE_TOPRIGHT].h = TILE_HEIGHT;

            _spriteClips[TILE_RIGHT].x = 240;
            _spriteClips[TILE_RIGHT].y = 80;
            _spriteClips[TILE_RIGHT].w = TILE_WIDTH;
            _spriteClips[TILE_RIGHT].h = TILE_HEIGHT;

            _spriteClips[TILE_BOTTOMRIGHT].x = 240;
            _spriteClips[TILE_BOTTOMRIGHT].y = 160;
            _spriteClips[TILE_BOTTOMRIGHT].w = TILE_WIDTH;
            _spriteClips[TILE_BOTTOMRIGHT].h = TILE_HEIGHT;

            _inited = true;
        }
        return true;
    }
    static bool loadTiles(const std::string& path, std::vector<Tile*>& tiles)
    {
        //The tile offsets
        int x = 0, y = 0;

        //Open the map
        std::ifstream map(path);

        if (map.fail()) //If the map couldn't be loaded
        {
            printf("Unable to load map file!\n");
            return false;
        }
        //Determines what kind of tile will be made
        int tileType = -1;
        int i = 0;
        //Initialize the tiles
        while (map >> tileType)
        {
            if (map.fail()) //If the was a problem in reading the map
            {
                //Stop loading map
                printf("Error loading map: Unexpected end of file!\n");
                return false;
            }

            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) //If the number is a valid tile number
            {
                tiles.emplace_back(new Tile(x, y, (TILE_TYPE)tileType));
            }
            else //If we don't recognize the tile type
            {
                //Stop loading map
                printf("Error loading map: Invalid tile type at %d!\n", i);
                return false;
            }

            //Move to next tile spot
            x += TILE_WIDTH;

            if (x >= LEVEL_WIDTH) //If we've gone too far
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }

            i++;
        }

        //Close the file
        map.close();


        return true;
    }
private:
    //The attributes of the tile
    SDL_Rect _box;

    //The tile type
    TILE_TYPE _type;

    //static
    static bool _inited;
    static LTexture _tilesSheet;
    static SDL_Rect _spriteClips[TOTAL_TILE_SPRITES];
};

