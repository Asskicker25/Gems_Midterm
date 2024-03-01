#include "Timer.h"

Timer& Timer::GetInstance()
{
    static Timer instance;
    return instance;
}

void Timer::SetCurrentTime(float currentTime)
{
    this->currentTime = currentTime;

    deltaTime = currentTime - lastFrameTime;

    if (deltaTime > MAX_DELTATIME) { deltaTime = MAX_DELTATIME; }

    elapsedTime += deltaTime;

    lastFrameTime = currentTime;
}
