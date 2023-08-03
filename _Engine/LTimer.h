#pragma once

#include <SDL/SDL.h>

class LTimer
{
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    Uint32 getTicks();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 _startTicks = 0;

    //The ticks stored when the timer was paused
    Uint32 _pausedTicks = 0;

    //The timer status
    bool _paused = false;
    bool _started = false;
};

