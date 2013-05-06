#include "Timer.h"

using namespace Ymir;

Timer::Timer(double time) : goalTime(time)
{
    passedTime = 0;
}

void Timer::update(double time)
{
    passedTime += time;
}

bool Timer::reached()
{
    if (passedTime >= goalTime) {
        return true;
    }
    return false;
}

void Timer::reset()
{
    passedTime = 0.0;
}
