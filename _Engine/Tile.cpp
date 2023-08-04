#include "Tile.h"
#include "Utils.h"

bool Tile::_inited = false;
LTexture Tile::_tilesSheet;
SDL_Rect Tile::_spriteClips[TOTAL_TILE_SPRITES];

Tile::Tile(int x, int y, float scale ,TILE_TYPE tileType)
    : _type(tileType)
{
    //Set the collision box
    _box = { x, y, int(TILE_TEXTURE_WIDTH * scale), int(TILE_TEXTURE_HEIGHT * scale) };
    _scale = scale;
}

void Tile::render(SDL_Rect& camera)
{
    //If the tile is on screen
    if (checkCollision(camera, _box))
    {
        //Show the tile
        _tilesSheet.render(_box.x - camera.x, _box.y - camera.y, &_spriteClips[_type], _scale);
    }
}


