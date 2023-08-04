#pragma once
#include "TileData.h"

//The dimensions of the level
static const int LEVEL_WIDTH = 9 * TILE_TEXTURE_WIDTH;
static const int LEVEL_HEIGHT = 6 * TILE_TEXTURE_HEIGHT;

//Screen dimension constants
const static int SCREEN_WIDTH = 1280;
const static int SCREEN_HEIGHT = 720;

//Differences
static const int LEVEL_SCREEN_WIDTH_DIF = LEVEL_WIDTH - SCREEN_WIDTH;
static const int LEVEL_SCREEN_HEIGHT_DIF = LEVEL_HEIGHT - SCREEN_HEIGHT;


