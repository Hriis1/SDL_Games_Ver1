#include "LTimer.h"

LTimer::LTimer()
{
}

void LTimer::start()
{
    //Start the timer
    _started = true;

    //Unpause the timer
    _paused = false;

    //Get the current clock time
    _startTicks = SDL_GetTicks();
    _pausedTicks = 0;
}

void LTimer::stop()
{
    _started = false;
    _paused = false;

    _startTicks = 0;
    _pausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if (_started && !_paused)
    {
        //Pause the timer
        _paused = true;

        //Calculate the paused ticks
        _pausedTicks = SDL_GetTicks() - _startTicks;
        _startTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if (_started && _paused)
    {
        //Unpause the timer
        _paused = false;

        //Reset the starting ticks
        _startTicks = SDL_GetTicks() - _pausedTicks;

        //Reset the paused ticks
        _pausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (_started)
    {
        //If the timer is paused
        if (_paused)
        {
            //Return the number of ticks when the timer was paused
            time = _pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - _startTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	return _started;
}

bool LTimer::isPaused()
{
	return _paused;
}
