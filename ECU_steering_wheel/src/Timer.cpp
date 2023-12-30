#include "Timer.hpp"

void Timer::ReFresh()
{
    preTime = GetTime();
}

bool Timer::IsTimeOut()
{
    bool isTimeOut = false;

    if (GetTime() - preTime > interval)
    {
        isTimeOut = true;
        preTime = GetTime();
    }

    return isTimeOut;
}