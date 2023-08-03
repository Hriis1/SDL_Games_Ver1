#include "LTexture.h"


LTexture::LTexture()
{
}

LTexture::~LTexture()
{
	//Dealocate memory
	free();
}

void LTexture::initRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
}

bool LTexture::createBlank32bit(int width, int height, SDL_TextureAccess access)
{
    //Get rid of preexisting texture
    free();

    //Create uninitialized texture
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, access, width, height);
    if (_texture == NULL)
    {
        printf("Unable to create streamable blank texture! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
        _width = width;
        _height = height;
    }

    return _texture != NULL;
}

bool LTexture::loadFromFile(const std::string& path, SDL_Window* window)
{
    //Load pixels
    if (!loadPixelsFromFile(path, window))
    {
        printf("Failed to load pixels for %s!\n", path.c_str());
    }
    else
    {
        //Load texture from pixels
        if (!loadFromPixels())
        {
            printf("Failed to texture from pixels from %s!\n", path.c_str());
        }
    }

    //Return success
    return _texture != NULL;
}

bool LTexture::loadFromRenderedText(const std::string& textureText, SDL_Color textColor, TTF_Font* font)
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        _texture = SDL_CreateTextureFromSurface(_renderer, textSurface);
        if (_texture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            _width = textSurface->w; 
            _height = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return _texture != NULL;
}

bool LTexture::loadPixelsFromFile(const std::string& path, SDL_Window* window)
{
    //Free preexisting assets
    free();

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Convert surface to display format
        _surfacePixels = SDL_ConvertSurfaceFormat(loadedSurface, SDL_GetWindowPixelFormat(window), 0);
        if (_surfacePixels == NULL)
        {
            printf("Unable to convert loaded surface to display format! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            _width = _surfacePixels->w;
            _height = _surfacePixels->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return _surfacePixels != NULL;
}

bool LTexture::loadFromPixels()
{
    //Only load if pixels exist
    if (_surfacePixels == NULL)
    {
        printf("No pixels loaded!");
    }
    else
    {
        //Color key image
        SDL_SetColorKey(_surfacePixels, SDL_TRUE, SDL_MapRGB(_surfacePixels->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        _texture = SDL_CreateTextureFromSurface(_renderer, _surfacePixels);
        if (_texture == NULL)
        {
            printf("Unable to create texture from loaded pixels! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            _width = _surfacePixels->w;
            _height = _surfacePixels->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(_surfacePixels);
        _surfacePixels = NULL;
    }

    //Return success
    return _texture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if (_texture != NULL)
    {
        SDL_DestroyTexture(_texture);
        _texture = NULL;
        _width = 0;
        _height = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    /*
    Modulate texture
    255 = full of this color
    0 = none of this color
    */
    SDL_SetTextureColorMod(_texture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(_texture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(_texture, alpha);
}

void LTexture::setAsRenderTarget()
{
    //Make self render target
    SDL_SetRenderTarget(_renderer, _texture);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, _width, _height };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(_renderer, _texture, clip, &renderQuad, angle, center, flip);
}

Uint32* LTexture::getPixels32()
{
    Uint32* pixels = NULL;

    if (_surfacePixels != NULL)
    {
        pixels = static_cast<Uint32*>(_surfacePixels->pixels);
    }

    return pixels;
}

Uint32 LTexture::getPixel32(Uint32 x, Uint32 y)
{
    if (_surfacePixels != NULL)
    {
        //Convert the pixels to 32 bit
        Uint32* pixels = static_cast<Uint32*>(_surfacePixels->pixels);

        //Get the pixel requested
        return pixels[(y * getPitch32()) + x];
    }

    return 0;
    
}

Uint32 LTexture::getPitch32()
{
    Uint32 pitch = 0;

    if (_surfacePixels != NULL)
    {
        //get the pixels per pitch not bytes per pitch by deviding by 4
        pitch = _surfacePixels->pitch / 4;
    }

    return pitch;
}

void LTexture::copyRawPixels32(void* pixels)
{
    //Texture is locked
    if (_rawPixels != NULL)
    {
        //Copy to locked pixels
        memcpy(_rawPixels, pixels, _rawPitch * _height);
    }
}

bool LTexture::lockTexture()
{
    //Texture is already locked
    if (_rawPixels != NULL)
    {
        printf("Texture is already locked!\n");
        return false;
    }
    //Lock texture
    else
    {
        if (SDL_LockTexture(_texture, NULL, &_rawPixels, &_rawPitch) != 0)
        {
            printf("Unable to lock texture! %s\n", SDL_GetError());
            return false;
        }
    }

    return true;
}

bool LTexture::unlockTexture()
{
    //Texture is not locked
    if (_rawPixels == NULL)
    {
        printf("Texture is not locked!\n");
        return false;
    }
    //Unlock texture
    else
    {
        SDL_UnlockTexture(_texture);
        _rawPixels = NULL;
        _rawPitch = 0;
    }

    return true;
}
