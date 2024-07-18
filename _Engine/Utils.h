#pragma once
#include <SDL/SDL.h>

//A circle stucture
template<typename T>
struct Circle
{
    T x, y;
    T r;
};

//Calculates distance squared between two points
static float distanceSquared(float x1, float y1, float x2, float y2)
{
    float xDist = x1 - x2;
    float yDist = y1 - y2;

    return pow(xDist, 2) + pow(yDist, 2);
}

//Check collision between 2 rects
static bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //The actual collision check
    return (rightA >= leftB && leftA <= rightB && bottomA >= topB && topA <= bottomB);
}

static bool checkCollision(const SDL_FRect& a, const SDL_FRect& b)
{
    //The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //The actual collision check
    if (rightA >= leftB && leftA <= rightB && bottomA >= topB && topA <= bottomB)
        return true;

    return false;
}

//Circle/Circle collision detector
template<typename T>
static bool checkCollision(Circle<T>& a, Circle<T>& b)
{
    //Calculate total radius squared
    float totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

//Circle/Box collision detector
template<typename T>
static bool checkCollision(Circle<T>& a, const SDL_FRect& b)
{
    //Closest point on collision box
    float cX, cY;

    //Find closest x of the box
    if (a.x < b.x) //Circles center is on the left
    {
        cX = b.x;
    }
    else if (a.x > b.x + b.w) //Circles center is on the right
    {
        cX = b.x + b.w;
    }
    else //Circles center is inside the boxes X 
    {
        cX = a.x;
    }

    //Find the closest y of the box
    if (a.y < b.y) //Circles center is above the box
    {
        cY = b.y;
    }
    else if (a.y > b.y + b.h) //Circles center is below the box
    {
        cY = b.y + b.h;
    }
    else //Circles center is inside the boxes Y
    {
        cY = a.y;
    }

    if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r) //If the distance between the closest point of the box and the center of the circle is less than the radius^2 of the circle there is a collision
        return true;

    return false;
}

template<typename T>
static bool checkCollision(Circle<T>& a, const SDL_Rect& b)
{
    //Closest point on collision box
    float cX, cY;

    //Find closest x of the box
    if (a.x < b.x) //Circles center is on the left
    {
        cX = b.x;
    }
    else if (a.x > b.x + b.w) //Circles center is on the right
    {
        cX = b.x + b.w;
    }
    else //Circles center is inside the boxes X 
    {
        cX = a.x;
    }

    //Find the closest y of the box
    if (a.y < b.y) //Circles center is above the box
    {
        cY = b.y;
    }
    else if (a.y > b.y + b.h) //Circles center is below the box
    {
        cY = b.y + b.h;
    }
    else //Circles center is inside the boxes Y
    {
        cY = a.y;
    }

    if (distanceSquared(a.x, a.y, cX, cY) < a.r * a.r) //If the distance between the closest point of the box and the center of the circle is less than the radius^2 of the circle there is a collision
        return true;

    return false;
}

static bool checkCollisionWithLevel(const SDL_FRect& _collisionRect, const std::vector<SDL_FRect>& level)
{
    for (size_t i = 0; i < level.size(); i++)
        if (checkCollision(_collisionRect, level[i]))
            return true;

    return false;
}

static void collideWithLevel(const SDL_FRect& _collisionRect, float& _xPos, float& _yPos, const std::vector<SDL_FRect>& level)
{
    for (size_t i = 0; i < level.size(); i++)
    {
        if (checkCollision(_collisionRect, level[i]))
        {
            //Player box values
            int playerleft = _collisionRect.x;
            int playerRight = _collisionRect.x + _collisionRect.w;
            int playerTop = _collisionRect.y;
            int playerBot = _collisionRect.y + _collisionRect.h;

            //wall box values
            int wallleft = level[i].x;
            int wallRight = level[i].x + level[i].w;
            int wallTop = level[i].y;
            int wallBot = level[i].y + level[i].h;

            int horizontalDistance = std::min(std::abs(playerRight - wallleft),
                std::abs(playerleft - wallRight));

            int verticalDistance = std::min(std::abs(playerBot - wallTop),
                std::abs(playerTop - wallBot));

            if (horizontalDistance < verticalDistance) {
                // Resolve horizontal collision
                float box1CenterX = playerleft + (_collisionRect.w / 2.0f);
                float box2CenterX = wallleft + (level[i].w / 2.0f);
                if (box1CenterX < box2CenterX) //coming from the left
                {
                    _xPos += wallleft - playerRight;
                }
                else if (box1CenterX > box2CenterX) {
                    _xPos += wallRight - playerleft;
                }
            }
            else if (horizontalDistance > verticalDistance) {
                float box1CenterY = playerTop + (_collisionRect.h / 2.0f);
                float box2CenterY = wallTop + (level[i].h / 2.0f);
                // Resolve vertical collision
                if (box1CenterY < box2CenterY) //coming from the top
                {
                    _yPos = wallTop - _collisionRect.h;

                }
                else {
                    _yPos = wallBot;
                }
            }

        }
    }
}


