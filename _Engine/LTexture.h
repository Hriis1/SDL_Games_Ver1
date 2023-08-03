#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>

class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //init funcs
    void initRenderer(SDL_Renderer* renderer);

    //Creates blank 33bit texture
    bool createBlank32bit(int width, int height, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING);

    //Loads image at specified path
    bool loadFromFile(const std::string& path, SDL_Window* window);

    //Creates image from font string
    bool loadFromRenderedText(const std::string& textureText, SDL_Color textColor, TTF_Font* font);

    //Loads image into pixel buffer
    bool loadPixelsFromFile(const std::string& path, SDL_Window* window);

    //Creates image from preloaded pixels
    bool loadFromPixels();


    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Set self as render target
    void setAsRenderTarget();

    //Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Pixel accessors
    Uint32* getPixels32();
    Uint32 getPixel32(Uint32 x, Uint32 y);
    Uint32 getPitch32();
    void copyRawPixels32(void* pixels);
    bool lockTexture();
    bool unlockTexture();

    //getters
    int getWidth() const
    {
        return _width;
    }
    int getHeight() const
    {
        return _height;
    }
    

private:
    //The renderer
    SDL_Renderer* _renderer = NULL;

    //The actual hardware texture
    SDL_Texture* _texture = NULL;

    //Surface pixels
    SDL_Surface* _surfacePixels = NULL;

    //Image dimensions
    int _width = 0;
    int _height = 0;

    //Raw pixels
    void* _rawPixels = NULL;
    int _rawPitch = 0;
};

