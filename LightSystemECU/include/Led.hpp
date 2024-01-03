#ifndef LED_HPP
#define LED_HPP

#include <iostream>
#include <string>
#include <StateControl.hpp>

#include <Arduino.h>

enum LedState
{
    OFF,
    ON
};

class Led
{
private:
    std::string name;
    uint8_t pin;

    StateControl<LedState> state;
    
public:
    Led(std::string name, uint8_t pin);

    bool TurnLed(LedState);

    static void Blinking(void* parameter)
    {
         
    }
    //blinking
};

#endif