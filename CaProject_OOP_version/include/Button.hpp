#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>
#include <Timer.hpp>

//button 
#define BT_ON 0
#define BT_OFF 1

class Button
{
private:
    int pin;
    int state;
    int pre_state;

    Timer *timer;

public:
    Button(int pin, Timer *timer) : pin(pin)
    {
        state = 0;
        pre_state = 0;

        timer = timer;
        pinMode(pin, INPUT_PULLUP);
    }

    int GetButtonState()
    {
        return state;
    }

    void ReadButton()
    {
        int reading_state = digitalRead(pin);

        if (reading_state != pre_state)
        {
            timer->ResetTimer();
        }

        if(timer->IsTimeOut())
        {
            //update the state
            state = pre_state;
        }

        pre_state = reading_state;
    }
};

#endif BUTTOn_HPP
