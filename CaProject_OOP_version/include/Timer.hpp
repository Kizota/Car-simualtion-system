#ifndef TIMER_HPP
#define TIMER_HPP

#include <iostream>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class Timer
{
private:
    uint32_t now;
    uint32_t pre_time;
    uint32_t interval;

public:
    Timer(uint32_t now, uint32_t interval)
    {
    }
    
    //reset the timing again
    uint8_t ResetTimer()
    {
        pre_time = now;
    }
    
    //notify when time is out
    uint8_t IsTimeOut()
    {
        uint8_t isTimeOut = false;
        now = GetMillis();
        
        //timing and reset whe timer out
        if (now - pre_time > interval)
        {
            isTimeOut = true;
            ResetTimer();
        }

        return isTimeOut;
    }
    
    //get time in ms with FreeRTOS - threadsafe
    static uint32_t GetMillis()
    {
        TickType_t tick_count = xTaskGetTickCount();

        uint32_t tick_rate = xPortGetTickRateHz();

        return tick_count * (1000) / tick_rate;
    }
};

#endif