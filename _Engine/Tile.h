#pragma once
#include <SDL/SDL.h>
#include <vector>
#include <fstream>

#include "GlobalData.h"
#include "LTexture.h"

//Tile constants
const static int TILE_WIDTH = 160;
const static int TILE_HEIGHT = 160;
const static int TOTAL_TILES = 192;

//The different tile sprites
static enum TILE_TYPE
{
    TILE_SKULL = 0, TILE_ROOTS, TILE_GRASS_LEFTEND, TILE_GRASS_RIGHTEND, TILE_GRASS_MIDDLE1, TILE_GRASS_MIDDLE2,
    TILE_DIRT_LEFTEND, TILE_DIRT_RIGHTEND, TILE_DIRT_MIDDLE1, TILE_DIRT_MIDDLE2, TILE_PLATFORM1, TILE_PLATFORM2,
    TOTAL_TILE_SPRITES
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

            if (!_tilesSheet.loadFromFile("../_Engine/tiles/spritesheet.png", window))
                return false;

            //init _spriteClips
            _spriteClips[TILE_SKULL].x = 0;
            _spriteClips[TILE_SKULL].y = 0;
            _spriteClips[TILE_SKULL].w = TILE_WIDTH;
            _spriteClips[TILE_SKULL].h = TILE_HEIGHT;

            _spriteClips[TILE_ROOTS].x = 161;
            _spriteClips[TILE_ROOTS].y = 0;
            _spriteClips[TILE_ROOTS].w = TILE_WIDTH;
            _spriteClips[TILE_ROOTS].h = TILE_HEIGHT;



            _spriteClips[TILE_GRASS_LEFTEND].x = 322;
            _spriteClips[TILE_GRASS_LEFTEND].y = 0;
            _spriteClips[TILE_GRASS_LEFTEND].w = TILE_WIDTH;
            _spriteClips[TILE_GRASS_LEFTEND].h = TILE_HEIGHT;

            _spriteClips[TILE_GRASS_RIGHTEND].x = 322;
            _spriteClips[TILE_GRASS_RIGHTEND].y = 161;
            _spriteClips[TILE_GRASS_RIGHTEND].w = TILE_WIDTH;
            _spriteClips[TILE_GRASS_RIGHTEND].h = TILE_HEIGHT;

            _spriteClips[TILE_GRASS_MIDDLE1].x = 161;
            _spriteClips[TILE_GRASS_MIDDLE1].y = 322;
            _spriteClips[TILE_GRASS_MIDDLE1].w = TILE_WIDTH;
            _spriteClips[TILE_GRASS_MIDDLE1].h = TILE_HEIGHT;

            _spriteClips[TILE_GRASS_MIDDLE2].x = 483;
            _spriteClips[TILE_GRASS_MIDDLE2].y = 322;
            _spriteClips[TILE_GRASS_MIDDLE2].w = TILE_WIDTH;
            _spriteClips[TILE_GRASS_MIDDLE2].h = TILE_HEIGHT;



            _spriteClips[TILE_DIRT_LEFTEND].x = 483;
            _spriteClips[TILE_DIRT_LEFTEND].y = 0;
            _spriteClips[TILE_DIRT_LEFTEND].w = TILE_WIDTH;
            _spriteClips[TILE_DIRT_LEFTEND].h = TILE_HEIGHT;
                              
            _spriteClips[TILE_DIRT_RIGHTEND].x = 0;
            _spriteClips[TILE_DIRT_RIGHTEND].y = 483;
            _spriteClips[TILE_DIRT_RIGHTEND].w = TILE_WIDTH;
            _spriteClips[TILE_DIRT_RIGHTEND].h = TILE_HEIGHT;
                              
            _spriteClips[TILE_DIRT_MIDDLE1].x = 0;
            _spriteClips[TILE_DIRT_MIDDLE1].y = 322;
            _spriteClips[TILE_DIRT_MIDDLE1].w = TILE_WIDTH;
            _spriteClips[TILE_DIRT_MIDDLE1].h = TILE_HEIGHT;
                              
            _spriteClips[TILE_DIRT_MIDDLE2].x = 483;
            _spriteClips[TILE_DIRT_MIDDLE2].y = 161;
            _spriteClips[TILE_DIRT_MIDDLE2].w = TILE_WIDTH;
            _spriteClips[TILE_DIRT_MIDDLE2].h = TILE_HEIGHT;

            
            
            
            

            _spriteClips[TILE_PLATFORM1].x = 161;
            _spriteClips[TILE_PLATFORM1].y = 483;
            _spriteClips[TILE_PLATFORM1].w = TILE_WIDTH;
            _spriteClips[TILE_PLATFORM1].h = TILE_HEIGHT;

            _spriteClips[TILE_PLATFORM2].x = 322;
            _spriteClips[TILE_PLATFORM2].y = 483;
            _spriteClips[TILE_PLATFORM2].w = TILE_WIDTH;
            _spriteClips[TILE_PLATFORM2].h = TILE_HEIGHT;

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

            if (tileType == 99)//empty tile
            {

            }
            else if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) //If the number is a valid tile number
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

