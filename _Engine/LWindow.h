#pragma once
#include <SDL/SDL.h>
#include <memory>

#include "GlobalData.h"

class LWindow
{
public:
    //Intializes internals
    LWindow();

    //Creates window and inits its renderer
    bool init(int width, int height);

    //Handles window events
    void handleEvent(SDL_Event& e);

    //Focuses on window
    void focus();

    //Shows windows contents
    void render();

    //Deallocates internals
    void free();

    //getters
    SDL_Window* getSDLWindow() const
    {
        return _window;
    }

    int getWidth() const
    {
        return _width;
    }
    int getHeight() const
    {
        return _height;
    }

    bool hasMouseFocus() const
    {
        return _mouseFocus;
    }
    bool hasKeyboardFocus() const
    {
        return _keyboardFocus;
    }
    bool isMinimized() const
    {
        return _minimized;
    }

    bool isShown() const
    {
        return _shown;
    }

public:

    //The renderer for the window
    SDL_Renderer* renderer = NULL;

private:
    //Window data
    SDL_Window* _window = NULL;
    int _windowID = 0;
    int _windowDisplayID = 0;

    //Display data
    int _totalDisplays = 0;
    std::unique_ptr<SDL_Rect[]> _displayBounds;

    //Window dimensions
    int _width = 0;
    int _height = 0;

    //Window focus
    bool _mouseFocus = false;
    bool _keyboardFocus = false;
    bool _fullScreen = false;
    bool _minimized = false;
    bool _shown = false;
};

