#ifndef _LTIMER_HPP_INCLUDED
#define _LTIMER_HPP_INCLUDED

#include <SDL.h>

class LTimer{

public:

    LTimer();

    void start();
    void stop();
    void pause();
    void unpause();

    // Resets the timer
    void reset();

    Uint32 getTicks() const;
    double getTicksSeconds() const;

    bool isStarted() const;
    bool isPaused() const;

private:
    // Started and paused flags
    bool mStarted;
    bool mPaused;

    // Started and paused offsets
    int mStartedTicks;
    int mPausedTicks;
};

#endif // _LTIMER_HPP_INCLUDED
