#include "LWindow.h"
#include<sstream>

LWindow::LWindow()
{
}

bool LWindow::init()
{
    //Get number of displays
    _totalDisplays = SDL_GetNumVideoDisplays();

    //Get bounds of each display
    _displayBounds = new SDL_Rect[_totalDisplays];
    for (int i = 0; i < _totalDisplays; ++i)
    {
        SDL_GetDisplayBounds(i, &_displayBounds[i]);
    }

    //Create window
    _window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (_window != NULL)
    {
        _mouseFocus = true;
        _keyboardFocus = true;
        _width = SCREEN_WIDTH;
        _height = SCREEN_HEIGHT;

        //Create renderer for window
        renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == NULL)
        {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(_window);
            _window = NULL;
        }
        else
        {
            //Initialize renderer color
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //Grab window identifiers
            _windowID = SDL_GetWindowID(_window);
            _windowDisplayID = SDL_GetWindowDisplayIndex(_window);

            //Flag as opened
            _shown = true;
        }
    }

    return _window != NULL && renderer != NULL;
}

void LWindow::handleEvent(SDL_Event& e)
{
    //Caption update flag
    bool updateCaption = false;

    //If an event was detected for this window
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == _windowID)
    {
        switch (e.window.event)
        {
            //Window moved
        case SDL_WINDOWEVENT_MOVED:
            _windowDisplayID = SDL_GetWindowDisplayIndex(_window);
            updateCaption = true;
            break;

            //Window appeared
        case SDL_WINDOWEVENT_SHOWN:
            _shown = true;
            break;

            //Window disappeared
        case SDL_WINDOWEVENT_HIDDEN:
            _shown = false;
            break;

            //Get new dimensions and repaint on window size change
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            _width = e.window.data1;
            _height = e.window.data2;
            SDL_RenderPresent(renderer);
            break;

            //Repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(renderer);
            break;

            //Mouse entered window
        case SDL_WINDOWEVENT_ENTER:
            _mouseFocus = true;
            updateCaption = true;
            break;

            //Mouse left window
        case SDL_WINDOWEVENT_LEAVE:
            _mouseFocus = false;
            updateCaption = true;
            break;

            //Window has keyboard focus
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            _keyboardFocus = true;
            updateCaption = true;
            break;

            //Window lost keyboard focus
        case SDL_WINDOWEVENT_FOCUS_LOST:
            _keyboardFocus = false;
            updateCaption = true;
            break;

            //Window minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            _minimized = true;
            break;

            //Window maximized
        case SDL_WINDOWEVENT_MAXIMIZED:
            _minimized = false;
            break;

            //Window restored
        case SDL_WINDOWEVENT_RESTORED:
            _minimized = false;
            break;

            //Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(_window);
            break;
        }
    }
    else if (e.type == SDL_KEYDOWN) 
    {
        if (e.window.windowID == _windowID) //check if its correct window
        {
            //Display change flag
            bool switchDisplay = false;

            switch (e.key.keysym.sym)
            {
            case SDLK_RETURN:
                if (_fullScreen)
                {
                    SDL_SetWindowFullscreen(_window, SDL_FALSE);
                    _fullScreen = false;
                }
                else
                {
                    SDL_SetWindowFullscreen(_window, SDL_TRUE);
                    _fullScreen = true;
                    _minimized = false;
                }
                break;

            /*case SDLK_UP:
                ++_windowDisplayID;
                switchDisplay = true;
                break;

            case SDLK_DOWN:
                --_windowDisplayID;
                switchDisplay = true;
                break;*/
            }

            //Display needs to be updated
            if (switchDisplay)
            {
                //Bound display index
                if (_windowDisplayID < 0)
                {
                    _windowDisplayID = _totalDisplays - 1;
                }
                else if (_windowDisplayID >= _totalDisplays)
                {
                    _windowDisplayID = 0;
                }

                //Move window to center of next display
                SDL_SetWindowPosition(_window, _displayBounds[_windowDisplayID].x + (_displayBounds[_windowDisplayID].w - _width) / 2, _displayBounds[_windowDisplayID].y + (_displayBounds[_windowDisplayID].h - _height) / 2);
                updateCaption = true;
            }
        }
    }

    //Update window caption with new data
    if (updateCaption)
    {
        std::stringstream caption;
        caption << "SDL Tutorial - ID: " << _windowID << " Display: " << _windowDisplayID << " MouseFocus:" << ((_mouseFocus) ? "On" : "Off") << " KeyboardFocus:" << ((_keyboardFocus) ? "On" : "Off");
        SDL_SetWindowTitle(_window, caption.str().c_str());
    }

}

void LWindow::focus()
{
    //Restore window if needed
    if (!_shown)
    {
        SDL_ShowWindow(_window);
    }

    //Move window forward
    SDL_RaiseWindow(_window);
}

void LWindow::render()
{
}

void LWindow::free()
{
    if (_window != NULL)
        SDL_DestroyWindow(_window);

    if(renderer != NULL)
        SDL_DestroyRenderer(renderer);

    _mouseFocus = false;
    _keyboardFocus = false;
    _width = 0;
    _height = 0;
}
