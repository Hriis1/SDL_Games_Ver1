#include "Ghost.h"

const float Ghost::TEXTURE_SCALE = 2.0f;
const float Ghost::GHOST_HEIGHT = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_WIDTH = 16 * Ghost::TEXTURE_SCALE;
const float Ghost::GHOST_VEL = 200.0f;
LTexture Ghost::_redGhostSpriteSheet;
LTexture Ghost::_greenGhostSpriteSheet;
LTexture Ghost::_blueGhostSpriteSheet;
LTexture Ghost::_orangeGhostSpriteSheet;
LTexture Ghost::_yellowGhostSpriteSheet;
SDL_Rect Ghost::_spriteClips[Ghost_ANIMATION_FRAMES];

Ghost::Ghost(float xPos, float yPos, GhostType type)
    : _xPos(xPos), _yPos(yPos), _type(type)
{
    shiftColliders();
}

void Ghost::update(float deltaTime, const std::vector<SDL_FRect>& walls)
{
    //Update position
    _xPos += GHOST_VEL * deltaTime;
    shiftColliders();

    //Collide with walls
    for (size_t i = 0; i < walls.size(); i++)
    {
        if (checkCollision(_collisionRect, walls[i]))
        {
            //Ghost box values
            int ghostleft = _collisionRect.x;
            int ghostRight = _collisionRect.x + _collisionRect.w;
            int ghostTop = _collisionRect.y;
            int ghostBot = _collisionRect.y + _collisionRect.h;

            //wall box values
            int wallleft = walls[i].x;
            int wallRight = walls[i].x + walls[i].w;
            int wallTop = walls[i].y;
            int wallBot = walls[i].y + walls[i].h;

            int horizontalDistance = std::min(std::abs(ghostRight - wallleft),
                std::abs(ghostleft - wallRight));

            int verticalDistance = std::min(std::abs(ghostBot - wallTop),
                std::abs(ghostTop - wallBot));

            if (horizontalDistance < verticalDistance) {
                // Resolve horizontal collision
                float box1CenterX = ghostleft + (_collisionRect.w / 2.0f);
                float box2CenterX = wallleft + (walls[i].w / 2.0f);
                if (box1CenterX < box2CenterX) //coming from the left
                {
                    _xPos += wallleft - ghostRight;
                }
                else if (box1CenterX > box2CenterX) {
                    _xPos += wallRight - ghostleft;
                }
            }
            else if (horizontalDistance > verticalDistance) {
                float box1CenterY = ghostTop + (_collisionRect.h / 2.0f);
                float box2CenterY = wallTop + (walls[i].h / 2.0f);
                // Resolve vertical collision
                if (box1CenterY < box2CenterY) //coming from the top
                {
                    _yPos = wallTop - _collisionRect.h;

                }
                else {
                    _yPos = wallBot;
                }
            }
            shiftColliders();
        }
    }
}

bool Ghost::init(SDL_Renderer* renderer, SDL_Window* window)
{
    //init texture
    _redGhostSpriteSheet.initRenderer(renderer);
    _greenGhostSpriteSheet.initRenderer(renderer);
    _blueGhostSpriteSheet.initRenderer(renderer);
    _orangeGhostSpriteSheet.initRenderer(renderer);
    _yellowGhostSpriteSheet.initRenderer(renderer);

    if (!_redGhostSpriteSheet.loadFromFile("Assets/Textures/redGhost.png", window))
    {
        printf("Could not load red ghost texture\n");
        return false;
    }
    if (!_greenGhostSpriteSheet.loadFromFile("Assets/Textures/greenGhost.png", window))
    {
        printf("Could not load green ghost texture\n");
        return false;
    }
    if (!_blueGhostSpriteSheet.loadFromFile("Assets/Textures/blueGhost.png", window))
    {
        printf("Could not load blue ghost texture\n");
        return false;
    }
    if (!_orangeGhostSpriteSheet.loadFromFile("Assets/Textures/orangeGhost.png", window))
    {
        printf("Could not load orange ghost texture\n");
        return false;
    }
    if (!_yellowGhostSpriteSheet.loadFromFile("Assets/Textures/yellowGhost.png", window))
    {
        printf("Could not load yellow ghost texture\n");
        return false;
    }

    //init spriteclips
    int textureWidth = GHOST_WIDTH / TEXTURE_SCALE;
    int textureHeight = GHOST_HEIGHT / TEXTURE_SCALE;
    _spriteClips[0] = { 0,0, textureWidth, textureHeight };
    _spriteClips[1] = { textureWidth,0, textureWidth, textureHeight };
    _spriteClips[2] = { textureWidth * 2,0, textureWidth, textureHeight };
    _spriteClips[3] = { textureWidth * 3,0, textureWidth, textureHeight };
    _spriteClips[4] = { textureWidth * 4,0, textureWidth, textureHeight };
    _spriteClips[5] = { textureWidth * 5,0, textureWidth, textureHeight };
    _spriteClips[6] = { textureWidth * 6,0, textureWidth, textureHeight };
    _spriteClips[7] = { textureWidth * 7,0, textureWidth, textureHeight };

    return true;
}

void Ghost::render(int camX, int camY)
{
    SDL_Rect* currentClip = &_spriteClips[_animationFrame / 12];

    if (_type == GhostType::RED)
    {
        _redGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if(_type == GhostType::GREEN)
    {
        _greenGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::BLUE)
    {
        _blueGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::ORANGE)
    {
        _orangeGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }
    else if (_type == GhostType::YELLOW)
    {
        _yellowGhostSpriteSheet.render(_xPos - camX, _yPos - camY, currentClip, TEXTURE_SCALE, 0.0, NULL, SDL_FLIP_NONE);
    }

    //Go to next frame
    _animationFrame++;

    //Cycle animation
    if (_animationFrame / 12 >= Ghost_ANIMATION_FRAMES)
    {
        _animationFrame = 0;
    }
}

void Ghost::shiftColliders()
{
    _collisionRect.x = _xPos;
    _collisionRect.y = _yPos;
}

