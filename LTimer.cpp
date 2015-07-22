#include <SDL.h>
#include "LTimer.hpp"

LTimer::LTimer()
{
    mStarted = false;
    mPaused = false;

    mStartedTicks = 0;
    mPausedTicks = 0;
}

void LTimer::start()
{
    // If timer is not started, start the timer ( get started offset )
    if( !mStarted )
    {
        mStartedTicks = SDL_GetTicks();
        mStarted = true;
    }
}

void LTimer::stop()
{
    // If timer is stated, stop it and reset timer offsets
    if( mStarted )
    {
        mStarted = false;
        mPaused = false;
        mStartedTicks = 0;
        mPausedTicks = 0;
    }
}

void LTimer::pause()
{
    // If timer is started and is not paused, pause it ( get paused offset )
    if( mStarted && !mPaused )
    {
        mPausedTicks = SDL_GetTicks() - mStartedTicks;
        mStartedTicks = 0;
        mPaused = true;
    }
}

void LTimer::unpause()
{
    // If timer is started and paused, unpause it ( return started offset to normal )
    if( mStarted && mPaused )
    {
        mStartedTicks = SDL_GetTicks() - mPausedTicks;
        mPausedTicks = 0;
        mPaused = false;
    }
}

Uint32 LTimer::getTicks() const
{
    if( mStarted )
    {
        if( mPaused )
        {
            return mPausedTicks;
        }
        else
        {
            return SDL_GetTicks() - mStartedTicks;
        }
    }

    return 0;
}

double LTimer::getTicksSeconds() const
{
    double ticks = getTicks();
    ticks /= 1000.f;
    return ticks;
}

bool LTimer::isStarted() const
{
    return mStarted;
}

bool LTimer::isPaused() const
{
    return mPaused;
}

void LTimer::reset()
{
    stop();
    start();
}
