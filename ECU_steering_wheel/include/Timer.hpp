#ifndef TIMER_HPP
#define TIMER_HPP

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/*
   timer is developed to combined code logic with FreeRTOS - guarantee thread safe
*/
class Timer
{
private:
    uint32_t preTime;
    uint32_t interval;

public:
    Timer(int interval) : interval(interval), preTime(0)
    {
    }

    void ReFresh();

    bool IsTimeOut();

    // consider using RTC timer
    static uint32_t GetTime()
    {
        TickType_t tick = xTaskGetTickCount();
        TickType_t tickRate = portTICK_RATE_MS;

        return tick * tickRate;
    }
};

#endif